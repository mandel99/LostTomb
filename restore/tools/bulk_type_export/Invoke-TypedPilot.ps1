param([Parameter(Mandatory=$true)][string]$ProjectRoot,[Parameter(Mandatory=$true)][string]$Output,[Parameter(Mandatory=$true)][string]$InputFile)
$ErrorActionPreference='Stop'
$taskRoot=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$privateRoot=[IO.Path]::GetFullPath((Join-Path $taskRoot 'build/bulk-type-pilot'))
foreach($target in @($ProjectRoot,$Output)) { $resolved=[IO.Path]::GetFullPath($target);if(-not $resolved.StartsWith($privateRoot+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw 'Target must be inside build/bulk-type-pilot'} }
$sha=[Security.Cryptography.SHA256]::Create();$stream=[IO.File]::OpenRead($InputFile)
try{$inputHash=[BitConverter]::ToString($sha.ComputeHash($stream)).Replace('-','').ToLowerInvariant()}finally{$stream.Dispose();$sha.Dispose()}
if($inputHash -ne 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'){throw 'Wrong pinned input'}
if(Test-Path -LiteralPath (Join-Path $ProjectRoot 'TypedPilot.gpr')){throw 'Private project already exists; refusing overwrite or lock manipulation'}
New-Item -ItemType Directory -Force -Path $ProjectRoot | Out-Null
$env:JAVA_HOME='C:\Users\Daniel\.codex\tools\jdk-21.0.12.101-hotspot'
$env:GHIDRA_HEADLESS_MAXMEM='2G'
$headless=Join-Path $taskRoot '../ghidra_12.1.4_PUBLIC/support/analyzeHeadless.bat'
& $headless $ProjectRoot TypedPilot -import $InputFile -noanalysis -scriptPath $PSScriptRoot -postScript ExportTypedPilot.java $Output -max-cpu 2 -log (Join-Path $privateRoot 'headless.log') -scriptlog (Join-Path $privateRoot 'script.log')
if($LASTEXITCODE -ne 0){throw "Private headless exited $LASTEXITCODE"}
if(-not (Test-Path -LiteralPath $Output)){throw 'Headless did not produce typed facts; inspect private logs; no fallback to an existing project'}
