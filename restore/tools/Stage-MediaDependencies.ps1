param(
    [Parameter(Mandatory=$true)][string]$GameDirectory,
    [string]$DestinationDirectory=(Join-Path $PSScriptRoot '../build/c-only-debug/Debug')
)
$ErrorActionPreference='Stop'
$GameDirectory=(Resolve-Path -LiteralPath $GameDirectory).Path
$DestinationDirectory=(Resolve-Path -LiteralPath $DestinationDirectory).Path
$entries=@()
foreach($name in @('mss32.dll','binkw32.dll','smackw32.dll')) {
    $source=(Resolve-Path -LiteralPath (Join-Path $GameDirectory $name)).Path
    $bytes=[IO.File]::ReadAllBytes($source)
    if($bytes.Length -lt 64 -or [BitConverter]::ToUInt16($bytes,0) -ne 0x5a4d) {throw "Invalid media DLL: $source"}
    $pe=[BitConverter]::ToInt32($bytes,0x3c)
    if($pe -lt 0 -or $pe+24 -gt $bytes.Length -or [BitConverter]::ToUInt32($bytes,$pe) -ne 0x4550 -or [BitConverter]::ToUInt16($bytes,$pe+4) -ne 0x14c) {throw "Media DLL must be a Win32 PE: $source"}
    $digest=(Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash.ToLowerInvariant()
    $destination=Join-Path $DestinationDirectory $name
    if(Test-Path -LiteralPath $destination) {
        if((Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant() -ne $digest) {throw "A different media DLL already exists: $destination"}
    } else {Copy-Item -LiteralPath $source -Destination $destination}
    if((Get-FileHash -LiteralPath $destination -Algorithm SHA256).Hash.ToLowerInvariant() -ne $digest) {throw "Media DLL copy verification failed: $destination"}
    $entries+=[ordered]@{name=$name;source=$source;destination=$destination;bytes=$bytes.Length;sha256=$digest}
}
$report=[ordered]@{format=1;scope='Existing licensed media dependencies staged beside the standalone C executable';original_executable_copied=$false;original_game_executed=$false;entries=$entries}
$report | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $DestinationDirectory 'media-dependencies.json') -Encoding utf8
$entries | ForEach-Object {Write-Output ('Media dependency ready: {0}' -f $_.destination)}
