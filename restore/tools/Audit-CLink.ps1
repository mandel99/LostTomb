[CmdletBinding()]
param([string]$Library=(Join-Path $PSScriptRoot '../build/c-only/Release/pharaoh_restored.lib'),
      [string]$Output=(Join-Path $PSScriptRoot '../metadata/c-only-link-audit.json'))
$ErrorActionPreference='Stop'
$Library=(Resolve-Path -LiteralPath $Library).Path
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$installation=& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$compilerRoot=Join-Path $installation 'VC/Tools/MSVC'
$version=Get-ChildItem -LiteralPath $compilerRoot -Directory | Sort-Object Name -Descending | Select-Object -First 1
$dumpbin=Join-Path $version.FullName 'bin/Hostx64/x86/dumpbin.exe'
$defined=[Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
$references=[Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
& $dumpbin /nologo /symbols $Library | ForEach-Object {
    if ($_ -match '^\s*[0-9A-F]+\s+([0-9A-F]+)\s+(UNDEF|SECT[0-9A-F]+|ABS)\s+.*?\bExternal\s+\|\s+(\S+)') {
        $value=$Matches[1]; $section=$Matches[2]; $symbol=$Matches[3]
        if ($section -eq 'UNDEF' -and $value -match '^0+$') { [void]$references.Add($symbol) }
        else { [void]$defined.Add($symbol) }
    }
}
if ($LASTEXITCODE -ne 0) { throw 'COFF symbol inspection failed.' }
$external=@($references | Where-Object {-not $defined.Contains($_)} | Sort-Object)
$groups=[ordered]@{
    imported_system_or_crt=@($external | Where-Object {$_ -match '^__imp__'})
    audio_video=@($external | Where-Object {$_ -match 'AIL_|Bink|Smack'})
    recovered_runtime_or_other=@($external | Where-Object {$_ -notmatch '^__imp__|AIL_|Bink|Smack'})
}
$report=[ordered]@{
    format=1
    library=$Library
    library_sha256=(Get-FileHash -LiteralPath $Library -Algorithm SHA256).Hash.ToLowerInvariant()
    original_executable_read=$false
    external_defined_symbols=$defined.Count
    pending_link_symbols=$external.Count
    groups=$groups
    note='All procedure sources compile. These symbols must be supplied by libraries or reconstructed runtime when linking the complete game. A static archive alone does not establish a playable C-only executable.'
}
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $Output -Encoding utf8
Write-Output ('C-only archive: {0} definitions; {1} external link requirements. Report: {2}' -f $defined.Count,$external.Count,[IO.Path]::GetFullPath($Output))
