[CmdletBinding()]
param([ValidateSet('campaign','mission','map','load','duplicate','cancel','delete')][string[]]$Cases=@('campaign','mission','duplicate','cancel','delete'))
$ErrorActionPreference='Stop'
$restoreRoot=[IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$fixtureRoot=Join-Path $restoreRoot 'build/debug-startup-game/Save'
$profile=Join-Path $fixtureRoot 'Restore.dat'
$saveFolder=Join-Path $fixtureRoot 'Restore'
$backup=Join-Path $restoreRoot 'artifacts/family-dialog-fixtures'
$ownedImage=Join-Path $restoreRoot 'build/c-only-debug/Debug/restore_link_check.exe'
$python='C:\Users\Daniel\AppData\Local\Programs\Python\Python312\python.exe'
$profileHash='7794376f0421356ed16a950de128b5029cc30d679b3f04c3f58760e86d242371'
function HashFile([string]$path) {
    $hashResult=Get-FileHash -LiteralPath $path -Algorithm SHA256 -ErrorAction Stop
    if(!$hashResult -or !$hashResult.Hash){throw "No hash returned for fixture: $path"}
    ([string]$hashResult.Hash).ToLowerInvariant()
}
if((HashFile $profile) -ne $profileHash -or (HashFile (Join-Path $backup 'Restore.dat')) -ne $profileHash){throw 'Fixture profile differs from its known C-created baseline'}
if(@(Get-ChildItem -LiteralPath $saveFolder -Force).Count){throw 'Integration requires an empty isolated Restore save folder'}
foreach($directory in @($fixtureRoot,$saveFolder)) {
    if((Get-Item -LiteralPath $directory).Attributes -band [IO.FileAttributes]::ReparsePoint){throw 'Integration refuses redirected fixture folders'}
}
if(@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $ownedImage}).Count){throw 'An owned C process is already running'}
$markerNames=@('restore-dialog-fixture.sav','restore-dialog-retained.txt')
$childHash=HashFile $ownedImage
$mapFolder=Join-Path $restoreRoot 'build/debug-startup-game/Maps'
$ownedMap=Join-Path $mapFolder 'restore-native-v160.map'
$mapCreated=$false;$ownedMapHash=$null
$saveCreated=$false;$ownedSaveHash=$null;$ownedSave=Join-Path $saveFolder 'restore-native.sav'
Push-Location -LiteralPath $restoreRoot
try {
    foreach($scenario in $Cases) {
        if($scenario -eq 'load') {
            $saveProof=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/mission-save-emulation.json') -Raw | ConvertFrom-Json
            $saveFixture=@($saveProof.saves | Where-Object {$_.path -eq 'artifacts/mission-chunks-fixtures/mission-00.sav'})
            if(!$saveProof.passed -or $saveFixture.Count -ne 1){throw 'SAV native proof is missing'}
            $ownedSaveHash=$saveFixture[0].sha256;$saveSource=Join-Path $restoreRoot $saveFixture[0].path
            if((HashFile $saveSource) -ne $ownedSaveHash -or (Test-Path -LiteralPath $ownedSave)){throw 'SAV fixture differs from native proof or already exists'}
            Copy-Item -LiteralPath $saveSource -Destination $ownedSave;$saveCreated=$true
        }
        if($scenario -eq 'map') {
            if((Get-Item -LiteralPath $mapFolder).Attributes -band [IO.FileAttributes]::ReparsePoint){throw 'Map integration refuses a redirected fixture folder'}
            if(@(Get-ChildItem -LiteralPath $mapFolder -Force).Count){throw 'Map integration requires an empty isolated Maps folder'}
            $mapProof=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/mission-map-emulation.json') -Raw | ConvertFrom-Json
            $mapFixture=@($mapProof.maps | Where-Object {$_.path -eq 'artifacts/mission-map-fixtures/mission-52.map'})
            if(!$mapProof.passed -or $mapFixture.Count -ne 1){throw 'Map native proof is missing'}
            $ownedMapHash=$mapFixture[0].sha256;$mapSource=Join-Path $restoreRoot $mapFixture[0].path
            if((HashFile $mapSource) -ne $ownedMapHash){throw 'Map fixture differs from native proof'}
            Copy-Item -LiteralPath $mapSource -Destination $ownedMap;$mapCreated=$true
        }
        if($scenario -in @('cancel','delete')) {
            foreach($name in $markerNames){Copy-Item -LiteralPath (Join-Path $backup $name) -Destination (Join-Path $saveFolder $name)}
        }
        $log=Join-Path $restoreRoot $(if($scenario -in @('campaign','mission','map','load')){"artifacts/menu-$scenario-integration.log"}else{"artifacts/menu-dialog-$scenario.log"})
        $script=Join-Path $PSScriptRoot 'Debug-Standalone.ps1'
        Set-Content -LiteralPath $log -Value ''
        $action=switch($scenario){'campaign'{'-TestFamilyHistory'} 'mission'{'-TestFamilyCustomMissions -TestMissionBack'} 'map'{'-TestFamilyCustomMissions -TestNonemptyCustomMission'} 'load'{'-TestFamilyLoadGame -TestLoadSaveBack'} 'duplicate'{'-TestFamilyDialog duplicate'} 'cancel'{'-TestFamilyDialog delete-cancel'} 'delete'{'-TestFamilyDialog delete-confirm'}}
        $commandArgs="-NoProfile -NonInteractive -ExecutionPolicy Bypass -File `"$script`" -ContinueFromTitle -SkipIntro -TraceStartup -TimeoutSeconds 45 $action -Log `"$log`""
        Set-Content -LiteralPath $log -Value '' -Encoding utf8
        $wrapper=Start-Process -FilePath (Get-Process -Id $PID).Path -ArgumentList $commandArgs -WindowStyle Hidden -PassThru
        if($null -eq $wrapper){throw 'No process handle returned for the owned diagnostic wrapper'}
        Write-Output "Started isolated C scenario: $scenario"
        $ready=$false;$deadline=[DateTime]::UtcNow.AddSeconds(25)
        do {
            Start-Sleep -Milliseconds 250
            if(Test-Path -LiteralPath $log) {
                $trace=[string](Get-Content -LiteralPath $log -Raw)
                if([string]::IsNullOrEmpty($trace)){continue}
                if($scenario -eq 'campaign') {$ready=$trace -match 'completed frame copied after rendering: mode=23'}
                elseif($scenario -eq 'map') {$ready=$trace -match 'completed frame copied after rendering: mode=61\b'}
                elseif($scenario -eq 'load') {
                    $responseOffset=$trace.LastIndexOf('load/save: cancel through C buttons')
                    $ready=$responseOffset -ge 0 -and $trace.Substring([Math]::Max(0,$responseOffset)) -match 'completed frame copied after rendering: mode=48\b'
                }
                elseif($scenario -eq 'mission') {
                    $responseOffset=$trace.LastIndexOf('mission selector: back through C buttons')
                    $ready=$responseOffset -ge 0 -and $trace.Substring([Math]::Max(0,$responseOffset)) -match 'completed frame copied after rendering: mode=48\b'
                }
                else {
                    $responseOffset=$trace.LastIndexOf('family dialog: respond')
                    $ready=$responseOffset -ge 0 -and $trace.Substring([Math]::Max(0,$responseOffset)) -match 'completed frame copied after rendering: mode=(47|54)\b'
                }
            }
            $wrapper.Refresh()
            if($wrapper.HasExited){throw "C diagnostic ended before capture: $scenario; see $log"}
        } while(!$ready -and [DateTime]::UtcNow -lt $deadline)
        if(!$ready){throw "Frame readiness deadline exceeded: $scenario"}
        $ownedChild=@(Get-CimInstance Win32_Process | Where-Object {$_.ExecutablePath -eq $ownedImage})
        if($ownedChild.Count -ne 1){throw 'Expected one owned C child'}
        $ownedDebugger=Get-CimInstance Win32_Process -Filter "ProcessId=$($ownedChild[0].ParentProcessId)"
        if($ownedDebugger.ParentProcessId -ne $wrapper.Id){throw 'Owned debugger has unexpected ancestry'}
        $kinds=if($scenario -eq 'campaign'){@('campaign','family','options')}elseif($scenario -eq 'mission'){@('mission','return')}elseif($scenario -eq 'map'){@('mission')}elseif($scenario -eq 'load'){@('load-save','load-save-selected','return')}else{@('dialog','return')}
        foreach($kind in $kinds) {
            $stem=if($scenario -eq 'campaign'){"menu-$kind-frame"}else{"$scenario-$kind-frame"}
            $image=if($scenario -eq 'campaign'){"artifacts/menu-$kind-frame.bmp"}else{"artifacts/$scenario-$kind.bmp"}
            if($scenario -eq 'mission' -and $kind -eq 'mission'){$stem='mission-selector-frame';$image='artifacts/mission-selector.bmp'}
            if($scenario -eq 'map'){$stem='map-selector-frame';$image='artifacts/map-selector.bmp'}
            & $python tools/capture_restore_framebuffer.py --pid $ownedChild[0].ProcessId --debugger-pid $ownedChild[0].ParentProcessId --completed-probe --completed-mode $kind --output $image --manifest "metadata/$stem.json" *> "artifacts/$scenario-$kind-capture.log"
            if($LASTEXITCODE -ne 0){throw "Capture failed: $scenario/$kind"}
        }
        if($scenario -in @('cancel','delete')) {
            $sav=Join-Path $saveFolder $markerNames[0];$other=Join-Path $saveFolder $markerNames[1]
            $facts=[ordered]@{child_exe_sha256=$childHash;other_file_sha256=(HashFile $other)}
            if($scenario -eq 'cancel') {
                $facts.profile_preserved=(HashFile $profile) -eq $profileHash
                $facts.sav_preserved=(HashFile $sav) -eq (HashFile (Join-Path $backup $markerNames[0]))
                $facts.other_file_preserved=(HashFile $other) -eq (HashFile (Join-Path $backup $markerNames[1]))
                $facts.profile_sha256=HashFile $profile;$facts.sav_sha256=HashFile $sav
            } else {
                $facts.profile_removed=!(Test-Path -LiteralPath $profile)
                $facts.sav_removed=!(Test-Path -LiteralPath $sav)
                $facts.other_file_preserved=(HashFile $other) -eq (HashFile (Join-Path $backup $markerNames[1]))
                $facts.directory_retained=Test-Path -LiteralPath $saveFolder -PathType Container
            }
            $factsName=if($scenario -eq 'cancel'){'metadata/delete-cancel-filesystem.json'}else{'metadata/delete-confirm-filesystem.json'}
            $facts | ConvertTo-Json | Set-Content -LiteralPath $factsName -Encoding UTF8
        }
        $wrapper.WaitForExit()
        $trace=Get-Content -LiteralPath $log -Raw
        if($trace -notmatch 'DEBUG TIMEOUT:' -or $trace -match 'Unreconstructed|EXCEPTION (?!80000003|406d1388)'){throw "Scenario failed: $scenario"}
        if(!(Test-Path -LiteralPath $profile)){Copy-Item -LiteralPath (Join-Path $backup 'Restore.dat') -Destination $profile}
        foreach($name in $markerNames) {
            $target=Join-Path $saveFolder $name
            if(Test-Path -LiteralPath $target) {
                if((HashFile $target) -ne (HashFile (Join-Path $backup $name))){throw 'Refusing to remove a changed marker'}
                Remove-Item -LiteralPath $target
            }
        }
        if($scenario -eq 'map') {
            if((HashFile $ownedMap) -ne $ownedMapHash){throw 'Refusing to remove a changed map fixture'}
            Remove-Item -LiteralPath $ownedMap;$mapCreated=$false
        }
        if($scenario -eq 'load') {
            if((HashFile $ownedSave) -ne $ownedSaveHash){throw 'Refusing to remove a changed SAV fixture'}
            Remove-Item -LiteralPath $ownedSave;$saveCreated=$false
        }
        Write-Output "Passed bounded C scenario: $scenario"
    }
} catch {
    Write-Output $_.ScriptStackTrace
    throw
} finally {
    if($saveCreated -and (Test-Path -LiteralPath $ownedSave)) {
        if((HashFile $ownedSave) -ne $ownedSaveHash){throw 'Refusing to remove a changed SAV fixture'}
        Remove-Item -LiteralPath $ownedSave
    }
    if($mapCreated -and (Test-Path -LiteralPath $ownedMap)) {
        if((HashFile $ownedMap) -ne $ownedMapHash){throw 'Refusing to remove a changed map fixture'}
        Remove-Item -LiteralPath $ownedMap
    }
    Pop-Location
}
