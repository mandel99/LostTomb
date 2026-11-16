[CmdletBinding()]
param(
    [string] $Configuration = 'Release',
    [string] $BuildDirectory = 'build/bootstrap-vs2022-win32'
)

$ErrorActionPreference = 'Stop'
$sourceDirectory = $PSScriptRoot
$buildPath = Join-Path $sourceDirectory $BuildDirectory

$cmakeCommand = Get-Command cmake -ErrorAction SilentlyContinue
if ($cmakeCommand) {
    $cmake = $cmakeCommand.Source
} else {
    $cmake = 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
    if (-not (Test-Path -LiteralPath $cmake -PathType Leaf)) {
        throw 'CMake was not found. Install CMake or Visual Studio C++ tools.'
    }
}

& $cmake -S $sourceDirectory -B $buildPath -G 'Visual Studio 17 2022' -A Win32
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed with exit code $LASTEXITCODE." }

& $cmake --build $buildPath --config $Configuration --target pharaoh_clean_bootstrap -- /m
if ($LASTEXITCODE -ne 0) { throw "Bootstrap build failed with exit code $LASTEXITCODE." }

$executable = Join-Path $buildPath "$Configuration/pharaoh_clean_bootstrap.exe"
if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) {
    throw "Build succeeded but expected executable is missing: $executable"
}

$file = Get-Item -LiteralPath $executable
$hash = (Get-FileHash -LiteralPath $executable -Algorithm SHA256).Hash.ToLowerInvariant()
Write-Output "Executable: $($file.FullName)"
Write-Output "Size: $($file.Length) bytes"
Write-Output "SHA-256: $hash"
Write-Output 'Launched: no'
