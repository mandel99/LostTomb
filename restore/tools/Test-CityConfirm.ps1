[CmdletBinding()]
param([ValidatePattern('^[a-z0-9-]+$')][string]$RunName='city-confirm',[switch]$DismissMessage)
$ErrorActionPreference='Stop'
$restoreRoot=[IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$fixtureRoot=Join-Path $restoreRoot 'build/debug-startup-game'
$saveFolder=Join-Path $fixtureRoot 'Save/Restore'
$profile=Join-Path $fixtureRoot 'Save/Restore.dat'
$ownedSave=Join-Path $saveFolder 'restore-native.sav'
$ownedImage=Join-Path $restoreRoot 'build/c-only-debug/Debug/restore_link_check.exe'
$log=Join-Path $restoreRoot "artifacts/$RunName.log"
function HashFile([string]$path){(Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()}
foreach($path in @($saveFolder,$fixtureRoot)){
    if((Get-Item -LiteralPath $path).Attributes -band [IO.FileAttributes]::ReparsePoint){throw 'City diagnostic refuses redirected fixture paths'}
}
if(@(Get-ChildItem -LiteralPath $saveFolder -Force).Count){throw 'City diagnostic requires an empty isolated Save/Restore folder'}
if((HashFile $profile) -ne '7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'){throw 'City diagnostic profile differs from the C-created baseline'}
if(@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $ownedImage}).Count){throw 'An owned C diagnostic is already running'}
$proof=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/mission-save-emulation.json') -Raw | ConvertFrom-Json
$fixture=@($proof.saves | Where-Object {$_.path -eq 'artifacts/mission-chunks-fixtures/mission-00.sav'})
if(!$proof.passed -or $fixture.Count -ne 1){throw 'Verified SAV fixture is unavailable'}
$source=Join-Path $restoreRoot $fixture[0].path
if((HashFile $source) -ne $fixture[0].sha256){throw 'City SAV fixture hash differs'}
$profileHash=HashFile $profile;$binaryHash=HashFile $ownedImage
Copy-Item -LiteralPath $source -Destination $ownedSave
$wrapper=$null
try {
    $script=Join-Path $PSScriptRoot 'Debug-Standalone.ps1'
    Set-Content -LiteralPath $log -Value '' -Encoding utf8
    $arguments="-NoProfile -NonInteractive -ExecutionPolicy Bypass -File `"$script`" -ContinueFromTitle -SkipIntro -TraceStartup -TestFamilyLoadGame -TestLoadSaveConfirm -TimeoutSeconds 45 -Log `"$log`""
    if($DismissMessage){$arguments+=' -TestMessageDismiss'}
    $wrapper=Start-Process -FilePath (Get-Process -Id $PID).Path -ArgumentList $arguments -WindowStyle Hidden -PassThru
    $captured=$false;$gameplayCaptured=$false;$deadline=[DateTime]::UtcNow.AddSeconds(60)
    do {
        $wrapper.Refresh()
        if(!$wrapper.HasExited -and !$captured -and (Test-Path -LiteralPath $log)) {
            $currentTrace=[string](Get-Content -LiteralPath $log -Raw)
            if($currentTrace -match 'completed frame copied after rendering: mode=(1|5)\b') {
                $child=@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $ownedImage})
                if($child.Count -ne 1){throw 'Expected one owned city C child'}
                $debugger=Get-CimInstance Win32_Process -Filter "ProcessId=$($child[0].ParentProcessId)"
                if($debugger.ParentProcessId -ne $wrapper.Id){throw 'City debugger ancestry differs'}
                & 'C:\Users\Daniel\AppData\Local\Programs\Python\Python312\python.exe' (Join-Path $PSScriptRoot 'capture_restore_framebuffer.py') --pid $child[0].ProcessId --debugger-pid $child[0].ParentProcessId --completed-probe --completed-mode city --output (Join-Path $restoreRoot "artifacts/$RunName-frame.bmp") --manifest (Join-Path $restoreRoot "metadata/$RunName-frame.json") *> (Join-Path $restoreRoot "artifacts/$RunName-frame-capture.log")
                if($LASTEXITCODE -ne 0){throw 'Completed city capture failed'}
                $captured=$true
            }
        }
        if($DismissMessage -and !$wrapper.HasExited -and !$gameplayCaptured -and (Test-Path -LiteralPath $log)) {
            $currentTrace=[string](Get-Content -LiteralPath $log -Raw)
            if($currentTrace -and $currentTrace.Contains('message: completed gameplay frame copied after OK')) {
                $child=@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $ownedImage})
                if($child.Count -ne 1){throw 'Expected one owned gameplay C child'}
                $debugger=Get-CimInstance Win32_Process -Filter "ProcessId=$($child[0].ParentProcessId)"
                if($debugger.ParentProcessId -ne $wrapper.Id){throw 'Gameplay debugger ancestry differs'}
                & 'C:\Users\Daniel\AppData\Local\Programs\Python\Python312\python.exe' (Join-Path $PSScriptRoot 'capture_restore_framebuffer.py') --pid $child[0].ProcessId --debugger-pid $child[0].ParentProcessId --completed-probe --completed-mode city-gameplay --output (Join-Path $restoreRoot "artifacts/$RunName-gameplay-frame.bmp") --manifest (Join-Path $restoreRoot "metadata/$RunName-gameplay-frame.json") *> (Join-Path $restoreRoot "artifacts/$RunName-gameplay-frame-capture.log")
                if($LASTEXITCODE -ne 0){throw 'Completed gameplay capture failed'}
                $gameplayCaptured=$true
            }
        }
        if(!$wrapper.HasExited){Start-Sleep -Milliseconds 250}
    }while(!$wrapper.HasExited -and [DateTime]::UtcNow -lt $deadline)
    if(!$wrapper.HasExited){throw "Owned debugger has not completed; inspect PID $($wrapper.Id) before another run"}
    $trace=[string](Get-Content -LiteralPath $log -Raw)
    $facts=[ordered]@{child_exe_sha256=$binaryHash;original_game_process_executed=$false;computer_UI_automation_used=$false;
        debugger_wrapper_exit=$wrapper.ExitCode;confirm_reached=$trace.Contains('load/save: confirm through C buttons');
        exceptions=@([regex]::Matches($trace,'EXCEPTION ([0-9a-fA-F]{8})') | ForEach-Object {$_.Groups[1].Value});
        profile_preserved=(HashFile $profile) -eq $profileHash;fixture_preserved=(HashFile $ownedSave) -eq $fixture[0].sha256;
        standalone_gameplay_proven=$false;completed_city_frame_proven=$captured;
        completed_frame_manifest=$(if($captured){"metadata/$RunName-frame.json"}else{$null});log="artifacts/$RunName.log";
        message_dismissal_requested=[bool]$DismissMessage;message_OK_dispatched=$trace.Contains('message: dismiss OK through C small buttons');
        completed_gameplay_frame_proven=$gameplayCaptured;gameplay_frame_manifest=$(if($gameplayCaptured){"metadata/$RunName-gameplay-frame.json"}else{$null})}
    $facts.passed=$captured -and $facts.confirm_reached -and $facts.profile_preserved -and $facts.fixture_preserved -and
        $facts.exceptions.Count -eq 1 -and $facts.exceptions[0] -eq '80000003' -and
        $trace.Contains('DEBUG TIMEOUT:') -and !$trace.Contains('EXIT ')
    if($DismissMessage){$facts.passed=$facts.passed -and $gameplayCaptured -and $facts.message_OK_dispatched}
    $facts | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $restoreRoot "metadata/$RunName.json") -Encoding UTF8
    Get-Content -LiteralPath $log -Tail 34
    if(!$facts.passed){throw "Owned C city diagnostic failed; inspect metadata/$RunName.json and artifacts/$RunName.log"}
} finally {
    # A capture/readiness error must not remove the SAV while its child is
    # still loading. Allow the already bounded owned debugger to finish.
    if($wrapper -and !$wrapper.HasExited){
        $cleanupDeadline=[DateTime]::UtcNow.AddSeconds(60)
        do {
            Start-Sleep -Milliseconds 250
            $wrapper.Refresh()
        }while(!$wrapper.HasExited -and [DateTime]::UtcNow -lt $cleanupDeadline)
    }
    if(@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $ownedImage}).Count){
        throw 'City diagnostic preserves its SAV because the owned C child is still running; inspect it before another run'
    }
    if(Test-Path -LiteralPath $ownedSave){
        if((HashFile $ownedSave) -ne $fixture[0].sha256){throw 'City diagnostic preserved a modified SAV instead of removing it'}
        Remove-Item -LiteralPath $ownedSave
    }
}
