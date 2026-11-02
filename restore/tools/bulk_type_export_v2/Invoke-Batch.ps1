param([Parameter(Mandatory=$true)][string]$RunRoot)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$private=[IO.Path]::GetFullPath((Join-Path $root 'build/bulk-type-batch'))
$run=[IO.Path]::GetFullPath($RunRoot);if(-not $run.StartsWith($private+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw 'Nonprivate run'}
$project=Join-Path $run 'project';if(Test-Path -LiteralPath (Join-Path $project 'TypedBatch.gpr')){throw 'Existing project refused; no lock removal'}
$nativeInput=Join-Path $root 'build/smoke-game/Pharaoh.exe';$hash=[Security.Cryptography.SHA256]::Create();$stream=[IO.File]::OpenRead($nativeInput);try{$digest=[BitConverter]::ToString($hash.ComputeHash($stream)).Replace('-','').ToLowerInvariant()}finally{$stream.Dispose();$hash.Dispose()};if($digest-ne 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'){throw 'Wrong pinned input'}
[IO.Directory]::CreateDirectory($project)|Out-Null
$env:JAVA_HOME='C:\Users\Daniel\.codex\tools\jdk-21.0.12.101-hotspot';$env:GHIDRA_HEADLESS_MAXMEM='3G'
& (Join-Path $root '../ghidra_12.1.4_PUBLIC/support/analyzeHeadless.bat') $project TypedBatch -import $nativeInput -noanalysis -scriptPath $PSScriptRoot -postScript ExportTypedBatch.java (Join-Path $run 'seed.json') (Join-Path $run 'facts') -max-cpu 2 -log (Join-Path $run 'headless.log') -scriptlog (Join-Path $run 'script.log')
if($LASTEXITCODE-ne 0){throw "Private headless exited $LASTEXITCODE"};if(-not(Test-Path -LiteralPath (Join-Path $run 'facts/index.json'))){throw 'No complete index; inspect private logs; no existing-project fallback'}
