[CmdletBinding()]
param(
    [string]$OriginalDirectory='C:\Program Files (x86)\GOG.com\Pharaoh',
    [string]$DestinationDirectory=(Join-Path $PSScriptRoot '../build/debug-startup-game')
)
$ErrorActionPreference='Stop'
$source=Join-Path ([IO.Path]::GetFullPath($OriginalDirectory)) 'mission editor guide.txt'
$destinationRoot=[IO.Path]::GetFullPath($DestinationDirectory)
if(!(Test-Path -LiteralPath $source -PathType Leaf)){throw "Missing editor guide: $source"}
if(!(Test-Path -LiteralPath $destinationRoot -PathType Container)){throw "Missing asset fixture: $destinationRoot"}
$destination=Join-Path $destinationRoot 'mission editor guide.txt'
$sourceHash=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash.ToLowerInvariant()
if(Test-Path -LiteralPath $destination){
    $existingHash=(Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant()
    if($existingHash -ne $sourceHash){throw 'The destination guide differs; refusing to overwrite it'}
}else{Copy-Item -LiteralPath $source -Destination $destination}
$destinationHash=(Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant()
if($destinationHash -ne $sourceHash){throw 'Editor guide copy hash mismatch'}
[ordered]@{source=$source;destination=$destination;sha256=$destinationHash;bytes=(Get-Item -LiteralPath $destination).Length;originalExeRequired=$false;externalApplicationLaunched=$false}|ConvertTo-Json
