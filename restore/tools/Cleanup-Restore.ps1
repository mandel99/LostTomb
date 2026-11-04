[CmdletBinding()]
param(
    [Parameter(Mandatory)][string]$PlanPath,
    [switch]$Apply
)
$ErrorActionPreference='Stop'
$restoreRoot=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..')).TrimEnd('\')
$plan=Get-Content -LiteralPath $PlanPath -Raw | ConvertFrom-Json -AsHashtable
if (-not [string]::Equals($restoreRoot,[IO.Path]::GetFullPath($plan.root).TrimEnd('\'),[StringComparison]::OrdinalIgnoreCase)) {
    throw 'The plan belongs to another restore directory.'
}
$prefix=$restoreRoot+'\'
$blocked=[Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
foreach ($protected in $plan.protected_files_sha256.Keys) {
    $protectedPath=[IO.Path]::GetFullPath([IO.Path]::Combine($restoreRoot,$protected))
    if (-not $protectedPath.StartsWith($prefix,[StringComparison]::OrdinalIgnoreCase)) {
        throw "Protected path escapes restore: $protectedPath"
    }
    while ($protectedPath.Length -ge $restoreRoot.Length) {
        [void]$blocked.Add($protectedPath)
        if ($protectedPath.Equals($restoreRoot,[StringComparison]::OrdinalIgnoreCase)) { break }
        $protectedPath=[IO.Path]::GetDirectoryName($protectedPath)
    }
}
$targets=[Collections.Generic.List[object]]::new()
$checkedPaths=[Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
$activePaths=@($plan.active_prefixes | ForEach-Object { [IO.Path]::GetFullPath([IO.Path]::Combine($restoreRoot,$_)) })
foreach ($target in $plan.targets) {
    $absolute=[IO.Path]::GetFullPath([IO.Path]::Combine($restoreRoot,$target.path))
    if (-not $absolute.StartsWith($prefix,[StringComparison]::OrdinalIgnoreCase)) {
        throw "Target escapes restore: $absolute"
    }
    foreach ($activePath in $activePaths) {
        if ($absolute.Equals($activePath,[StringComparison]::OrdinalIgnoreCase) -or
            $absolute.StartsWith($activePath+'\',[StringComparison]::OrdinalIgnoreCase) -or
            $activePath.StartsWith($absolute+'\',[StringComparison]::OrdinalIgnoreCase)) {
            throw "Target overlaps active directory: $absolute"
        }
    }
    $probe=$absolute
    while ($probe.Length -gt $restoreRoot.Length -and -not $checkedPaths.Contains($probe)) {
        $attributes=[IO.File]::GetAttributes($probe)
        if ($attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Reparse point in deletion path: $probe" }
        [void]$checkedPaths.Add($probe)
        $probe=[IO.Path]::GetDirectoryName($probe)
    }
    if ($blocked.Contains($absolute)) { throw "Target contains a protected file: $absolute" }
    if ($target.kind -notin @('file','directory')) { throw "Unknown target type: $absolute" }
    $isDirectory=[bool]([IO.File]::GetAttributes($absolute) -band [IO.FileAttributes]::Directory)
    if (($target.kind -eq 'directory') -ne $isDirectory) { throw "Target type changed: $absolute" }
    if ($isDirectory) {
        $pending=[Collections.Generic.Stack[string]]::new()
        $pending.Push($absolute)
        while ($pending.Count) {
            foreach ($entry in [IO.Directory]::EnumerateFileSystemEntries($pending.Pop())) {
                $attributes=[IO.File]::GetAttributes($entry)
                if ($attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Reparse point below target: $entry" }
                if ($attributes -band [IO.FileAttributes]::Directory) { $pending.Push($entry) }
            }
        }
    }
    $targets.Add(@{path=$absolute;kind=$target.kind})
}
Write-Output ('Validated {0} targets inside {1}; planned {2:N3} GiB.' -f $targets.Count,$restoreRoot,($plan.delete_bytes/1GB))
if (-not $Apply) { return }

function Assert-ProtectedFiles {
    foreach ($entry in $plan.protected_files_sha256.GetEnumerator()) {
        $path=Join-Path $restoreRoot $entry.Key
        if (-not (Test-Path -LiteralPath $path -PathType Leaf)) { throw "Protected file missing: $path" }
        if ((Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash -ne $entry.Value) {
            throw "Protected file changed: $path"
        }
    }
}
Assert-ProtectedFiles
$drive=[IO.DriveInfo]::new([IO.Path]::GetPathRoot($restoreRoot))
$freeBefore=$drive.AvailableFreeSpace
$removed=0
foreach ($target in $targets) {
    # All absolute paths, ancestors, protected descendants and links were
    # checked above. Native PowerShell performs each deletion end to end.
    if ($target.kind -eq 'directory') {
        Remove-Item -LiteralPath $target.path -Recurse -Force
    } else {
        Remove-Item -LiteralPath $target.path -Force
    }
    $removed++
    if ($removed % 1000 -eq 0) { Write-Output "Removed $removed / $($targets.Count) targets." }
}
Assert-ProtectedFiles
$receipt=[ordered]@{
    completed_at=[DateTime]::UtcNow.ToString('o')
    restore_root=$restoreRoot
    plan_sha256=(Get-FileHash -LiteralPath $PlanPath -Algorithm SHA256).Hash.ToLowerInvariant()
    deleted_targets=$removed
    deleted_files=$plan.delete_files
    deleted_bytes=$plan.delete_bytes
    disk_free_before=$freeBefore
    disk_free_after=$drive.AvailableFreeSpace
    protected_files_unchanged=$plan.protected_files_sha256.Count
    active_prefixes=$plan.active_prefixes
    host_game_launched=$false
}
$receiptPath=Join-Path ([IO.Path]::GetDirectoryName([IO.Path]::GetFullPath($PlanPath))) 'receipt.json'
$receipt | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $receiptPath -Encoding utf8
Write-Output ('Removed {0} files, {1:N3} GiB; all {2} protected files unchanged.' -f $plan.delete_files,($plan.delete_bytes/1GB),$plan.protected_files_sha256.Count)
