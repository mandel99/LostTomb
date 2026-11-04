[CmdletBinding()]
param([string]$GameDirectory='C:\Program Files (x86)\GOG.com\Pharaoh',
      [switch]$SmokeGame,
      [string]$BuildDirectory,
      [switch]$PrintExecutable)
$ErrorActionPreference='Stop'
$executable=Join-Path $PSScriptRoot 'artifacts/windows/Release/PharaohRestore.exe'
if ($BuildDirectory) {
    $executable=Join-Path ([IO.Path]::GetFullPath($BuildDirectory)) 'PharaohRestore.exe'
} else {
    $current=Join-Path $PSScriptRoot 'artifacts/current-build.json'
    if (Test-Path -LiteralPath $current) {
        $selection=Get-Content -Raw -LiteralPath $current | ConvertFrom-Json
        $executable=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot $selection.executable))
    }
}
if (-not (Test-Path -LiteralPath $executable)) { throw 'The default build compiles C sources only. A game build requires the separate build.ps1 -Hybrid -OriginalExe PATH workflow.' }
if ($PrintExecutable) { Write-Output $executable; return }
if ($SmokeGame) {
    $GameDirectory=& (Join-Path $PSScriptRoot 'tools/Prepare-SmokeGame.ps1')
}
Start-Process -FilePath $executable -ArgumentList @('--game-dir',('"'+$GameDirectory+'"')) -WorkingDirectory $GameDirectory -WindowStyle Normal
