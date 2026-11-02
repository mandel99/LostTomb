[CmdletBinding()]
param([ValidateSet('Debug','Release')][string]$Configuration = 'Release', [switch]$Verify, [switch]$BuildVerification, [switch]$BuildDebugger, [switch]$BuildGame,
      [switch]$Hybrid, [switch]$VerifyOriginal,
      [string]$OriginalExe,
      [string]$BuildDirectory,
      [string]$RuntimeOutputRoot)
$ErrorActionPreference = 'Stop'
if (($Hybrid -or $VerifyOriginal) -and -not $OriginalExe) {
    throw 'OriginalExe is required only when explicitly requesting -Hybrid or -VerifyOriginal.'
}
if (-not $BuildDirectory) {
    $BuildDirectory=Join-Path $PSScriptRoot $(if ($Hybrid -or $VerifyOriginal) {'build/cmake'} else {'build/c-only'})
}
$BuildDirectory=[IO.Path]::GetFullPath($BuildDirectory)
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) { throw 'Visual Studio C++ Build Tools are required.' }
$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $installation) { throw 'Install the Visual Studio Desktop development with C++ workload.' }
$cmake = Join-Path $installation 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
if (-not (Test-Path -LiteralPath $cmake)) { $cmake = (Get-Command cmake -ErrorAction Stop).Source }
$major = (& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationVersion).Split('.')[0]
$generator = switch ($major) { '18' {'Visual Studio 18 2026'} '17' {'Visual Studio 17 2022'} '16' {'Visual Studio 16 2019'} default { throw "Unsupported Visual Studio major version: $major" } }
$originalOption = if ($VerifyOriginal) { "-DRESTORE_ORIGINAL_EXE=$OriginalExe" } else { '-DRESTORE_ORIGINAL_EXE=' }
$gameOption = if ($Hybrid) { "-DRESTORE_GAME_EXE=$OriginalExe" } else { '-DRESTORE_GAME_EXE=' }
$hybridOption = if ($Hybrid) { '-DRESTORE_BUILD_HYBRID=ON' } else { '-DRESTORE_BUILD_HYBRID=OFF' }
$outputOptions=@()
if ($RuntimeOutputRoot) {
    $RuntimeOutputRoot=[IO.Path]::GetFullPath($RuntimeOutputRoot)
    $outputOptions=@("-DRESTORE_RUNTIME_OUTPUT_ROOT=$RuntimeOutputRoot")
}
& $cmake -S $PSScriptRoot -B $BuildDirectory -G $generator -A Win32 $originalOption $gameOption $hybridOption @outputOptions
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed.' }
$targetOptions=@()
if (-not $Hybrid -and -not $Verify -and -not $VerifyOriginal -and -not $BuildVerification) {
    # The link check pulls in the C library and its data dependencies. Build the
    # large verification suite only when explicitly requested.
    $targetOptions=@('--target','restore_link_check')
}
& $cmake --build $BuildDirectory --config $Configuration @targetOptions --parallel 8
if ($LASTEXITCODE -ne 0) { throw 'C build failed. See compiler diagnostics above.' }
if ($BuildGame) {
    & $cmake --build $BuildDirectory --config $Configuration --target restore_game --parallel 8
    if ($LASTEXITCODE -ne 0) { throw 'C game build failed.' }
    Write-Host "Experimental C game: $(Join-Path $BuildDirectory "$Configuration/PharaohRestoreC.exe")"
}
if ($BuildDebugger) {
    & $cmake --build $BuildDirectory --config $Configuration --target restore_debug --parallel 8
    if ($LASTEXITCODE -ne 0) { throw 'Debugger helper build failed.' }
}
Write-Host "Built: $(Join-Path $BuildDirectory "$Configuration/pharaoh_restored.lib")"
if ($Verify -or $VerifyOriginal) {
    & (Join-Path (Split-Path $cmake) 'ctest.exe') --test-dir $BuildDirectory -C $Configuration --output-on-failure --timeout 30
    if ($LASTEXITCODE -ne 0) { throw 'C verification failed.' }
}
if (-not $Hybrid) {
    $sourceBuild=[ordered]@{
        kind='compiled-C-sources'
        originalExeRequired=[bool]$VerifyOriginal
        configuration=$Configuration
        library=[IO.Path]::GetRelativePath($PSScriptRoot,(Join-Path $BuildDirectory "$Configuration/pharaoh_restored.lib"))
        wholeProgramLinkCheck=[IO.Path]::GetRelativePath($PSScriptRoot,(Join-Path $BuildDirectory "$Configuration/restore_link_check.exe"))
        allRecoveredObjectsLinked=$true
        verified=[bool]($Verify -or $VerifyOriginal)
        standalonePlayableGame=$false
        procedureSources=(Get-Content -LiteralPath (Join-Path $BuildDirectory 'source-build.json') -Raw | ConvertFrom-Json).procedure_sources
    }
    New-Item -ItemType Directory -Force -Path (Join-Path $PSScriptRoot 'artifacts') | Out-Null
    $sourceBuild | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'artifacts/current-source-build.json') -Encoding utf8
    Write-Host 'C source build completed. No hybrid game was built or launched.'
    return
}
$cacheLine=Get-Content (Join-Path $BuildDirectory 'CMakeCache.txt') |
    Where-Object { $_ -match '^RESTORE_RUNTIME_OUTPUT_ROOT:PATH=' } | Select-Object -First 1
if ($cacheLine) {
    $outputRoot=$cacheLine.Substring($cacheLine.IndexOf('=')+1)
    $gameExecutable=Join-Path $outputRoot "$Configuration/PharaohRestore.exe"
    if (Test-Path -LiteralPath $gameExecutable) {
        $current=[ordered]@{
            executable=[IO.Path]::GetRelativePath($PSScriptRoot,$gameExecutable)
            configuration=$Configuration
            verified=[bool]($Verify -or $VerifyOriginal)
            executableSha256=(Get-FileHash -LiteralPath $gameExecutable -Algorithm SHA256).Hash.ToLowerInvariant()
            runtimeSha256=(Get-FileHash -LiteralPath (Join-Path (Split-Path $gameExecutable) 'PharaohRestoreRuntime.dll') -Algorithm SHA256).Hash.ToLowerInvariant()
        }
        New-Item -ItemType Directory -Force -Path (Join-Path $PSScriptRoot 'artifacts') | Out-Null
        $current | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'artifacts/current-build.json') -Encoding utf8
        Write-Host "Hybrid executable: $gameExecutable"
    }
}
