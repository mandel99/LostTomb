[CmdletBinding()]
param([string]$OriginalDirectory='C:\Program Files (x86)\GOG.com\Pharaoh')
$ErrorActionPreference='Stop'
$destination=Join-Path (Split-Path $PSScriptRoot) 'build/smoke-game'
New-Item -ItemType Directory -Force -Path $destination | Out-Null
$files=@('Pharaoh.exe','Pharaoh.inf','Pharaoh.ini','Pharaohmap.inf','Language.inf',
    'Pharaoh2.emp','Pharaoh_MM.eng','Pharaoh_Text.eng','mission1.pak','campaign.txt',
    'eventmsg.txt','music.txt','auto reason phrases.txt','trade_recommends.txt','mission editor guide.txt',
    'BINKW32.DLL','SMACKW32.DLL','mss32.dll','MP3DEC.ASI','mssb16.tsk')
foreach ($file in $files) {
    $source=Join-Path $OriginalDirectory $file
    if (Test-Path -LiteralPath $source) { Copy-Item -LiteralPath $source -Destination (Join-Path $destination $file) }
}
foreach ($pattern in @('Figure_model*.txt','Pharaoh_Model_*.txt','Tax_Sentiment_Model_*.txt','*.M3D')) {
    Get-ChildItem -LiteralPath $OriginalDirectory -Filter $pattern -File | ForEach-Object {
        Copy-Item -LiteralPath $_.FullName -Destination (Join-Path $destination $_.Name)
    }
}
foreach ($directory in @('Data','AUDIO','BINKS','Missions')) {
    $source=Join-Path $OriginalDirectory $directory
    $link=Join-Path $destination $directory
    if (-not (Test-Path -LiteralPath $link) -and (Test-Path -LiteralPath $source)) {
        New-Item -ItemType Junction -Path $link -Target $source | Out-Null
    }
}
New-Item -ItemType Directory -Force -Path (Join-Path $destination 'Save'),(Join-Path $destination 'Maps') | Out-Null
# Only the isolated copy is changed, at the executable-verified window-mode field.
$settings=Join-Path $destination 'Pharaoh.inf'
if (Test-Path -LiteralPath $settings) {
    $bytes=[IO.File]::ReadAllBytes($settings)
    if ($bytes.Length -eq 564) {
        [BitConverter]::GetBytes([int]1).CopyTo($bytes,4)
        [IO.File]::WriteAllBytes($settings,$bytes)
    }
}
Write-Output $destination
