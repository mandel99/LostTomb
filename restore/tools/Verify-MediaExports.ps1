[CmdletBinding()]
param([Parameter(Mandatory=$true)][string]$MediaDirectory)
$ErrorActionPreference='Stop'
$projectRoot=Split-Path $PSScriptRoot
$entries=(Get-Content -LiteralPath (Join-Path $projectRoot 'metadata/media-bindings.json') -Raw | ConvertFrom-Json).entries
$dumpbin=(Get-Command dumpbin.exe -ErrorAction SilentlyContinue).Source
if (-not $dumpbin) {
    $vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
    $installation=& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    $compilerVersion=(Get-Content -LiteralPath (Join-Path $installation 'VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt') -Raw).Trim()
    $dumpbin=Join-Path $installation "VC/Tools/MSVC/$compilerVersion/bin/Hostx64/x86/dumpbin.exe"
}
$results=@()
foreach ($group in ($entries | Group-Object dll)) {
    $dllPath=Join-Path ([IO.Path]::GetFullPath($MediaDirectory)) $group.Name
    if (-not (Test-Path -LiteralPath $dllPath)) {throw "Missing media library: $dllPath"}
    $exports=@(& $dumpbin /nologo /exports $dllPath)
    if ($LASTEXITCODE -ne 0) {throw "Cannot inspect exports: $dllPath"}
    $exportNames=@($exports | ForEach-Object {if ($_ -match '^\s+\d+\s+[0-9A-F]+\s+[0-9A-F]+\s+(\S+)') {$Matches[1]}})
    foreach ($entry in $group.Group) {
        $results+=[ordered]@{dll=$entry.dll; export=$entry.export; found=($exportNames -ccontains $entry.export)}
    }
}
$missing=@($results | Where-Object {-not $_.found})
$report=[ordered]@{method='static PE export inspection; DLLs not loaded'; checked=$results.Count; missing=$missing.Count; entries=$results}
$report | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $projectRoot 'metadata/media-export-check.json') -Encoding utf8
if ($missing.Count) {throw ($missing | ConvertTo-Json -Compress)}
Write-Host "Verified $($results.Count) multimedia exports without launching the game or loading the DLLs."
