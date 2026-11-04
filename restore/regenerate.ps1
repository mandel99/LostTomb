[CmdletBinding()]
param([string]$OriginalExe = 'C:\Program Files (x86)\GOG.com\Pharaoh\Pharaoh.exe',
      [switch]$SkipExport)
$ErrorActionPreference = 'Stop'
$expected = 'FDE7CD6542AB00E3952D0366C0C5C52DDE59A70160280F172E24D41406D33C8D'
if ((Get-FileHash -LiteralPath $OriginalExe -Algorithm SHA256).Hash -ne $expected) {
    throw 'The executable does not match the Pharaoh 2.1 reference binary.'
}
if (-not $SkipExport) {
    $node = Join-Path $PSScriptRoot '../PharaohRewrite/artifacts/tools/rea/node-v24.22.1-win-x64/node.exe'
    if (-not (Test-Path -LiteralPath $node)) {
        throw 'The pinned REA runtime is missing. Run ../PharaohRewrite/tools/Setup-Rea.ps1.'
    }
    & $node --max-old-space-size=4096 (Join-Path $PSScriptRoot 'tools/export_rea.mjs') $OriginalExe
    if ($LASTEXITCODE -ne 0) { throw 'REA export failed; retained batches can be resumed.' }
}
& python (Join-Path $PSScriptRoot 'tools/combine_rea.py')
if ($LASTEXITCODE -ne 0) { throw 'REA export is incomplete.' }
& python (Join-Path $PSScriptRoot 'tools/prepare_c.py') (Join-Path $PSScriptRoot 'raw/Pharaoh.rea.c')
if ($LASTEXITCODE -ne 0) { throw 'C generation failed.' }
& (Join-Path $PSScriptRoot 'build.ps1') -Verify
