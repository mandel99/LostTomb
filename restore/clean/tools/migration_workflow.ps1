[CmdletBinding()]
param(
    [ValidateSet('discover', 'plan', 'verify')]
    [string] $Mode = 'verify',
    [string[]] $Source = @(),
    [int] $Top = 40,
    [switch] $IncludeBridges,
    [switch] $AsJson
)

$ErrorActionPreference = 'Stop'

$cleanRoot = Split-Path -Parent $PSScriptRoot
$restoreRoot = Split-Path -Parent $cleanRoot
$bindingPath = Join-Path $restoreRoot 'metadata/GameCurrentReferenceBindings.json'
$mapPath = Join-Path $cleanRoot 'evidence/function-map.json'
$cmakePath = Join-Path $cleanRoot 'CMakeLists.txt'
$libraryPath = Join-Path $cleanRoot 'build/Release/pharaoh_game.lib'
$bridgePattern = 'Restore|LegacyBindings|Original[0-9A-Fa-f]|FUN_[0-9A-Fa-f]|#define\s+(?:MiU|EU|U|W|B)\s*\(|\b(?:MiU|EU|U|W|B)\s*\(|\(uintptr_t\)|__asm'
$productionPattern = 'RestoreFrame|LegacyBindings|Original[0-9A-Fa-f]|FUN_[0-9A-Fa-f]|\b(?:MiU|EU|U|W|B)\s*\(|\(\s*[^)]*\*\s*\)\s*0x[0-9A-Fa-f]{6,}'

function Normalize-Address([object] $value) {
    $text = [string] $value
    if ($text -match '^0[xX]([0-9A-Fa-f]{8})$') {
        return '0x{0:X8}' -f [Convert]::ToUInt32($Matches[1], 16)
    }
    return $text.ToUpperInvariant()
}

function Get-FunctionBody([string] $text, [string] $target) {
    if ([string]::IsNullOrWhiteSpace($target)) {
        return $null
    }
    $pattern = '(?m)\b' + [regex]::Escape($target) + '\s*\([^;{}]*\)\s*\{'
    $match = [regex]::Match($text, $pattern)
    if (-not $match.Success) {
        return $null
    }
    $start = $text.IndexOf('{', $match.Index)
    $depth = 0
    for ($index = $start; $index -lt $text.Length; $index++) {
        if ($text[$index] -eq '{') {
            $depth++
        } elseif ($text[$index] -eq '}') {
            $depth--
            if ($depth -eq 0) {
                return $text.Substring($match.Index, $index - $match.Index + 1)
            }
        }
    }
    return $null
}

function Write-Result([object] $value) {
    if ($AsJson) {
        $value | ConvertTo-Json -Depth 8
    } else {
        $value
    }
}

$bindings = Get-Content $bindingPath -Raw | ConvertFrom-Json
$map = Get-Content $mapPath -Raw | ConvertFrom-Json
$mappedAddresses = @{}
foreach ($function in $map.functions) {
    $mappedAddresses[(Normalize-Address $function.address)] = $true
}

if ($Mode -eq 'discover') {
    $candidates = @()
    $rows = @($bindings.Rows | Where-Object {
        $_.NativeEntry -and
        $_.Status -eq 'UniqueCompiledOwner' -and
        $_.Providers.Count -eq 1 -and
        $_.Providers[0].Source -like 'Source/*' -and
        -not $mappedAddresses.ContainsKey((Normalize-Address $_.NativeEntry))
    })
    foreach ($group in ($rows | Group-Object { $_.Providers[0].Source })) {
        $sourcePath = Join-Path $restoreRoot $group.Name
        if (-not (Test-Path -LiteralPath $sourcePath)) {
            continue
        }
        $text = Get-Content -LiteralPath $sourcePath -Raw
        $cleanRows = 0
        $bridgeRows = 0
        $unknownRows = 0
        foreach ($row in $group.Group) {
            $body = Get-FunctionBody $text ([string] $row.Target)
            if ($null -eq $body) {
                $unknownRows++
            } elseif ([regex]::IsMatch($body, $bridgePattern)) {
                $bridgeRows++
            } else {
                $cleanRows++
            }
        }
        if ($cleanRows -eq 0 -and -not $IncludeBridges) {
            continue
        }
        $candidates += [pscustomobject]@{
            functions = if ($IncludeBridges) { $group.Count } else { $cleanRows }
            excluded_bridges = $bridgeRows
            unresolved_bodies = $unknownRows
            lines = ($text -split "`n").Count
            source = $group.Name
            sha256 = (Get-FileHash -Algorithm SHA256 $sourcePath).Hash.ToLowerInvariant()
        }
    }
    Write-Result @($candidates | Sort-Object functions -Descending | Select-Object -First $Top)
    exit 0
}

if ($Mode -eq 'plan') {
    if ($Source.Count -eq 0) {
        throw 'Mode plan requires at least one -Source path relative to restore/.'
    }
    $plan = @()
    foreach ($sourceName in $Source) {
        $sourcePath = Join-Path $restoreRoot $sourceName
        if (-not (Test-Path -LiteralPath $sourcePath)) {
            throw "Source does not exist: $sourceName"
        }
        $sourceHash = (Get-FileHash -Algorithm SHA256 $sourcePath).Hash.ToLowerInvariant()
        $rows = @($bindings.Rows | Where-Object {
            $_.NativeEntry -and
            $_.Providers.Count -eq 1 -and
            $_.Providers[0].Source -eq $sourceName -and
            -not $mappedAddresses.ContainsKey((Normalize-Address $_.NativeEntry))
        } | Sort-Object NativeEntry)
        foreach ($row in $rows) {
            $text = Get-Content -LiteralPath $sourcePath -Raw
            $body = Get-FunctionBody $text ([string] $row.Target)
            $bodyFound = $null -ne $body
            $bodyBridgeHits = if ($bodyFound) {
                [regex]::Matches($body, $bridgePattern).Count
            } else {
                0
            }
            if ($bodyBridgeHits -ne 0 -and -not $IncludeBridges) {
                continue
            }
            $providerHash = ([string] $row.Providers[0].SourceSHA256).ToLowerInvariant()
            $plan += [pscustomobject]@{
                address = Normalize-Address $row.NativeEntry
                reference_function = $row.ReferenceFunction
                previous_target = $row.Target
                source = $sourceName
                source_sha256 = $sourceHash
                provider_hash_matches = $sourceHash -eq $providerHash
                function_body_found = $bodyFound
                function_bridge_hits = $bodyBridgeHits
                reference_body_sha256 = $row.ReferenceBodySHA256
                status = $row.Status
            }
        }
    }
    Write-Result $plan
    exit 0
}

$errors = [System.Collections.Generic.List[string]]::new()
$addresses = @()
$names = @()
foreach ($function in $map.functions) {
    $addresses += Normalize-Address $function.address
    $names += [string] $function.name
    if ([string] $function.source -notlike 'game/*') {
        $errors.Add("Mapped source is outside game/: $($function.source)")
    }
    if (-not (Test-Path -LiteralPath (Join-Path $cleanRoot $function.source))) {
        $errors.Add("Mapped source is missing: $($function.source)")
    }
}
foreach ($duplicate in @($addresses | Group-Object | Where-Object Count -gt 1)) {
    $errors.Add("Duplicate address: $($duplicate.Name)")
}
foreach ($duplicate in @($names | Group-Object | Where-Object Count -gt 1)) {
    $errors.Add("Duplicate name: $($duplicate.Name)")
}

$cmake = Get-Content $cmakePath -Raw
$productionFiles = @(Get-ChildItem (Join-Path $cleanRoot 'game') -Recurse -Filter '*.c')
foreach ($file in $productionFiles) {
    $relative = $file.FullName.Substring($cleanRoot.Length + 1).Replace('\', '/')
    if ($cmake -notmatch [regex]::Escape($relative)) {
        $errors.Add("Production C file is absent from CMake: $relative")
    }
}
foreach ($file in @(Get-ChildItem (Join-Path $cleanRoot 'game') -Recurse -File | Where-Object Extension -in '.c', '.h')) {
    $text = Get-Content -LiteralPath $file.FullName -Raw
    if ($text -match $productionPattern) {
        $errors.Add("Forbidden bridge/address construct: $($file.FullName.Substring($cleanRoot.Length + 1))")
    }
}

$bindingByAddress = @{}
foreach ($row in $bindings.Rows) {
    if ($row.NativeEntry) {
        $bindingByAddress[(Normalize-Address $row.NativeEntry)] = $row
    }
}
$batchFiles = @(Get-ChildItem (Join-Path $cleanRoot 'evidence') -Filter 'migration-batch-*.json' | Sort-Object Name)
$batchFunctionCount = 0
$latestBatch = $null
foreach ($batchFile in $batchFiles) {
    $batch = Get-Content $batchFile.FullName -Raw | ConvertFrom-Json
    $latestBatch = $batch
    $count = 0
    foreach ($group in $batch.groups) {
        $oldSource = Join-Path $restoreRoot $group.previous_source
        $newSource = Join-Path $cleanRoot $group.new_source
        if (-not (Test-Path -LiteralPath $oldSource)) {
            $errors.Add("Missing previous source in $($batchFile.Name): $($group.previous_source)")
        } else {
            $actualHash = (Get-FileHash -Algorithm SHA256 $oldSource).Hash.ToLowerInvariant()
            if ($actualHash -ne ([string] $group.previous_source_sha256).ToLowerInvariant()) {
                $errors.Add("Previous source hash mismatch in $($batchFile.Name): $($group.previous_source)")
            }
        }
        if (-not (Test-Path -LiteralPath $newSource)) {
            $errors.Add("Missing new source in $($batchFile.Name): $($group.new_source)")
        }
        foreach ($addressValue in $group.addresses) {
            $address = Normalize-Address $addressValue
            $count++
            if (-not $bindingByAddress.ContainsKey($address)) {
                $errors.Add("Address absent from binding inventory: $address")
            } elseif ($bindingByAddress[$address].Providers[0].Source -ne $group.previous_source) {
                $errors.Add("Owner mismatch for $address in $($batchFile.Name)")
            }
            if (-not $mappedAddresses.ContainsKey($address)) {
                $errors.Add("Batch address absent from function map: $address")
            }
        }
    }
    $batchFunctionCount += $count
    if ($count -ne [int] $batch.function_count) {
        $errors.Add("Function count mismatch in $($batchFile.Name): declared $($batch.function_count), found $count")
    }
}

if ($latestBatch -and $latestBatch.verification.library_sha256 -and
    (Test-Path -LiteralPath $libraryPath)) {
    $actualLibraryHash = (Get-FileHash -Algorithm SHA256 $libraryPath).Hash.ToLowerInvariant()
    if ($actualLibraryHash -ne ([string] $latestBatch.verification.library_sha256).ToLowerInvariant()) {
        $errors.Add('Current library hash differs from the latest migration record.')
    }
}

$result = [pscustomobject]@{
    ok = $errors.Count -eq 0
    mapped_functions = $map.functions.Count
    unique_addresses = @($addresses | Sort-Object -Unique).Count
    unique_names = @($names | Sort-Object -Unique).Count
    production_c_files = $productionFiles.Count
    migration_batches = $batchFiles.Count
    batch_functions = $batchFunctionCount
    errors = @($errors)
}
Write-Result $result
if ($errors.Count -ne 0) {
    exit 1
}
