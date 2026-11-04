[CmdletBinding()]
param([ValidateSet('Start','Status','Stop','Resume','Pilot')][string]$Action='Status')
$ErrorActionPreference='Stop'
$python=(Get-Command python -ErrorAction Stop).Source
$script=Join-Path $PSScriptRoot 'tools/ollama_batch.py'
$output=Join-Path $PSScriptRoot 'artifacts/ollama-batch'
if ($Action -eq 'Status') { & $python $script status; exit $LASTEXITCODE }
if ($Action -eq 'Stop') { & $python $script stop; exit $LASTEXITCODE }
if (-not (Test-Path -LiteralPath (Join-Path $output 'plan.json'))) {
    & $python $script plan
    if ($LASTEXITCODE -ne 0) { throw 'Batch plan failed.' }
}
$arguments=@('-u',('"'+$script+'"'))
if ($Action -eq 'Resume') { $arguments+='resume' } else { $arguments+='run' }
if ($Action -eq 'Pilot') { $arguments+=@('--limit','5') }
$stamp=Get-Date -Format 'yyyyMMdd-HHmmss-fff'
$stdout=Join-Path $output "worker-$stamp.stdout.log"
$stderr=Join-Path $output "worker-$stamp.stderr.log"
$process=Start-Process -FilePath $python -ArgumentList $arguments -WorkingDirectory $PSScriptRoot `
    -RedirectStandardOutput $stdout -RedirectStandardError $stderr -WindowStyle Hidden -PassThru
Write-Output "Batch worker PID: $($process.Id)"
Write-Output "Output: $stdout"
Write-Output "Errors: $stderr"
Write-Output 'The OS lock prevents two workers modifying this batch. Status is available through -Action Status.'
