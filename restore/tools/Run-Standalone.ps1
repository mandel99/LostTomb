param(
    [ValidateSet('Debug','Release')][string]$Configuration='Debug',
    [string]$AssetDirectory=(Join-Path $PSScriptRoot '../build/debug-startup-game')
)
$ErrorActionPreference='Stop'
$restoreRoot=Split-Path -Parent $PSScriptRoot
$build=if($Configuration -eq 'Debug'){'c-only-debug'}else{'c-only'}
$target=Join-Path $restoreRoot "build/$build/$Configuration/restore_link_check.exe"
foreach($file in @($target,(Join-Path (Split-Path $target) 'binkw32.dll'),(Join-Path (Split-Path $target) 'mss32.dll'),(Join-Path (Split-Path $target) 'smackw32.dll'))) {
    if(!(Test-Path -LiteralPath $file -PathType Leaf)){throw "Missing standalone runtime file: $file"}
}
$AssetDirectory=(Resolve-Path -LiteralPath $AssetDirectory).Path
$savedLayers=$env:__COMPAT_LAYER
try {
    $env:__COMPAT_LAYER=("$savedLayers 16BITCOLOR HIGHDPIAWARE DWM8And16BitMitigation").Trim()
    Push-Location -LiteralPath $AssetDirectory
    try {& $target '--game-startup-probe';$result=$LASTEXITCODE}
    finally {Pop-Location}
} finally {$env:__COMPAT_LAYER=$savedLayers}
exit $result
