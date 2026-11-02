[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$restoreRoot=[IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$fixtureRoot=Join-Path $restoreRoot 'build/debug-startup-game'
$save=Join-Path $fixtureRoot 'Save'
$target=Join-Path $save 'highscore.jas'
$profile=Join-Path $save 'Restore.dat'
$image=Join-Path $restoreRoot 'build/c-only-debug/Debug/restore_link_check.exe'
$log=Join-Path $restoreRoot 'artifacts/menu-high-scores-integration.log'
$python='C:\Users\Daniel\AppData\Local\Programs\Python\Python312\python.exe'
function HashFile([string]$path){(Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()}
foreach($folder in @($fixtureRoot,$save)){
    if((Get-Item -LiteralPath $folder).Attributes -band [IO.FileAttributes]::ReparsePoint){throw 'Scores integration refuses redirected fixture folders'}
}
if(@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $image}).Count){throw 'Owned C diagnostic is already running'}
$proof=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/high-scores-integration-fixture.json') -Raw | ConvertFrom-Json
$source=Join-Path $restoreRoot $proof.path
if((HashFile $source) -ne $proof.sha256 -or (Get-Item -LiteralPath $source).Length -ne 7600){throw 'Scores fixture differs from verified format'}
$profileHash=HashFile $profile
if($profileHash -ne '7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'){throw 'Profile differs from baseline'}
$original=$null;$originalHash=$null
if(Test-Path -LiteralPath $target){$original=[IO.File]::ReadAllBytes($target);$originalHash=HashFile $target;[IO.File]::WriteAllBytes((Join-Path $restoreRoot 'artifacts/high-scores-fixtures/before-integration.jas'),$original)}
$binaryHash=HashFile $image;$created=$false
Push-Location -LiteralPath $restoreRoot
try {
    Copy-Item -LiteralPath $source -Destination $target;$created=$true
    $script=Join-Path $PSScriptRoot 'Debug-Standalone.ps1'
    Set-Content -LiteralPath $log -Value '' -Encoding utf8
    $arguments="-NoProfile -NonInteractive -ExecutionPolicy Bypass -File `"$script`" -ContinueFromTitle -SkipIntro -TraceStartup -TestHighScores -TimeoutSeconds 45 -Log `"$log`""
    $wrapper=Start-Process -FilePath (Get-Process -Id $PID).Path -ArgumentList $arguments -WindowStyle Hidden -PassThru
    $deadline=[DateTime]::UtcNow.AddSeconds(30);$ready=$false
    do {
        Start-Sleep -Milliseconds 250
        if(Test-Path -LiteralPath $log){$trace=[string](Get-Content -LiteralPath $log -Raw);$offset=$trace.LastIndexOf('high scores: Escape back through C input');$ready=$offset -ge 0 -and $trace.Substring([Math]::Max(0,$offset)).Contains('completed frame copied after rendering: mode=0')}
        $wrapper.Refresh();if($wrapper.HasExited){throw 'Scores C child ended before capture; inspect its debug log'}
    }while(!$ready -and [DateTime]::UtcNow -lt $deadline)
    if(!$ready){throw "Scores frame deadline exceeded; inspect wrapper PID $($wrapper.Id) before rerunning"}
    $child=@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $image})
    if($child.Count -ne 1){throw 'Expected one owned C child'}
    $debugger=Get-CimInstance Win32_Process -Filter "ProcessId=$($child[0].ParentProcessId)"
    if($debugger.ParentProcessId -ne $wrapper.Id){throw 'Unexpected debugger ancestry'}
    foreach($kind in @('high-scores','high-scores-scrolled','high-scores-dragged','high-scores-return')){
        & $python tools/capture_restore_framebuffer.py --pid $child[0].ProcessId --debugger-pid $child[0].ParentProcessId --completed-probe --completed-mode $kind --output "artifacts/menu-$kind.bmp" --manifest "metadata/menu-$kind-frame.json" *> "artifacts/$kind-capture.log"
        if($LASTEXITCODE -ne 0){throw "Scores frame capture failed: $kind"}
    }
    $kinds=@('high-scores','high-scores-scrolled','high-scores-dragged','high-scores-return')
    $expectedFirst=@(0,1,5,5);$expectedModes=@(83,83,83,0)
    for($i=0;$i -lt $kinds.Count;$i++){
        $frame=Get-Content -LiteralPath "metadata/menu-$($kinds[$i])-frame.json" -Raw | ConvertFrom-Json
        if(!$frame.completed_frame_copy -or $frame.child_exe_sha256 -ne $binaryHash -or $frame.mode -ne $expectedModes[$i] -or $frame.high_scores_state.total -ne 25 -or $frame.high_scores_state.first -ne $expectedFirst[$i] -or $frame.high_scores_state.records_sha256 -ne $proof.sha256){throw "Scores captured state differs: $($kinds[$i])"}
    }
    if(!$wrapper.WaitForExit(60000)){throw "Scores debugger remains live at PID $($wrapper.Id)"}
    $trace=[string](Get-Content -LiteralPath $log -Raw)
    if($trace -notmatch 'DEBUG TIMEOUT:' -or $trace -match 'EXIT |Unreconstructed|EXCEPTION (?!80000003|406d1388)'){throw 'Scores run has unexpected termination/exception'}
    if((HashFile $target) -ne $proof.sha256){throw 'Scores file was modified by the read-only menu flow'}
    if((HashFile $profile) -ne $profileHash){throw 'Scores menu changed the isolated profile'}
    [ordered]@{child_exe_sha256=$binaryHash;original_game_process_executed=$false;computer_UI_automation_used=$false;passed=$true;expected_bounded_timeout=$true;fixture_sha256=$proof.sha256;fixture_preserved=$true;profile_preserved=$true;previous_scores_file_sha256=$originalHash;log='artifacts/menu-high-scores-integration.log'} | ConvertTo-Json | Set-Content -LiteralPath metadata/high-scores-integration-run.json -Encoding utf8
    Write-Output 'Passed C high scores: main-menu action,25 rows,down,drag,Escape return.'
} finally {
    if($created -and (Test-Path -LiteralPath $target)){
        if((HashFile $target) -ne $proof.sha256){throw 'Refusing to replace a modified scores fixture'}
        if($null -ne $original){[IO.File]::WriteAllBytes($target,$original);if((HashFile $target) -ne $originalHash){throw 'Original isolated scores file was not restored'}}
        else {Remove-Item -LiteralPath $target}
    }
    Pop-Location
}
