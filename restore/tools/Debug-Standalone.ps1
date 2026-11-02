param(
    [string]$AssetDirectory=(Join-Path $PSScriptRoot '../build/debug-startup-game'),
    [ValidateRange(1,300)][int]$TimeoutSeconds=15,
    [switch]$NoColorCompatibility,
    [switch]$ContinueFromTitle,
    [switch]$SkipIntro,
    [switch]$TestNameEntry,
    [switch]$TestCreateFamily,
    [switch]$TestFamilySelection,
    [switch]$TestFamilyHistory,
    [switch]$TestFamilyCustomMissions,
    [switch]$TestFamilyLoadGame,
    [switch]$TestLoadSaveBack,
    [switch]$TestLoadSaveConfirm,
    [switch]$TestMessageDismiss,
    [switch]$TestNonemptyCustomMission,
    [switch]$TestMissionBack,
    [switch]$TestHighScores,
    [switch]$TraceStartup,
    [ValidateSet('none','duplicate','delete-cancel','delete-confirm')][string]$TestFamilyDialog='none',
    [string]$Log=(Join-Path $PSScriptRoot '../artifacts/standalone-debug.log')
)
$ErrorActionPreference='Stop'
$restoreRoot=Split-Path -Parent $PSScriptRoot
$debugger=Join-Path $restoreRoot 'build/c-only-debug/Debug/restore_debug.exe'
$target=Join-Path $restoreRoot 'build/c-only-debug/Debug/restore_link_check.exe'
foreach($file in @($debugger,$target)) {if(!(Test-Path -LiteralPath $file -PathType Leaf)) {throw "Missing Debug build: $file"}}
$AssetDirectory=(Resolve-Path -LiteralPath $AssetDirectory).Path
$Log=[IO.Path]::GetFullPath($Log)
$savedTimeout=$env:RESTORE_DEBUG_TIMEOUT_MS
$savedCompatibility=$env:RESTORE_DEBUG_COMPAT16
$savedContinueTest=$env:RESTORE_STARTUP_CONTINUE_TEST
$savedSkipTest=$env:RESTORE_STARTUP_SKIP_VIDEO_TEST
$savedMenuActionTest=$env:RESTORE_MENU_ACTION_TEST
$savedMenuNameTest=$env:RESTORE_MENU_NAME_TEST
$savedMenuSubmitTest=$env:RESTORE_MENU_SUBMIT_TEST
$savedFamilySelectTest=$env:RESTORE_FAMILY_SELECT_TEST
$savedFamilyDialogTest=$env:RESTORE_FAMILY_DIALOG_TEST
$savedFamilyActionTest=$env:RESTORE_FAMILY_ACTION_TEST
$savedTraceStartup=$env:RESTORE_TRACE_STARTUP
$savedMissionBackTest=$env:RESTORE_MISSION_BACK_TEST
$savedLoadSaveBackTest=$env:RESTORE_LOAD_SAVE_BACK_TEST
$savedLoadSaveConfirmTest=$env:RESTORE_LOAD_SAVE_CONFIRM_TEST
$savedHighScoresTest=$env:RESTORE_HIGH_SCORES_TEST
$savedMessageDismissTest=$env:RESTORE_MESSAGE_DISMISS_TEST
$savedInternalInputOnly=$env:RESTORE_INTERNAL_INPUT_ONLY
try {
    $env:RESTORE_DEBUG_TIMEOUT_MS=[string]($TimeoutSeconds*1000)
    if($TraceStartup){$env:RESTORE_TRACE_STARTUP='1'}
    $env:RESTORE_DEBUG_COMPAT16=if($NoColorCompatibility){$null}else{'1'}
    $env:RESTORE_STARTUP_CONTINUE_TEST=if($ContinueFromTitle){'1'}else{$null}
    $env:RESTORE_STARTUP_SKIP_VIDEO_TEST=if($SkipIntro){'1'}else{$null}
    $env:RESTORE_HIGH_SCORES_TEST=if($TestHighScores){'1'}else{$null}
    $env:RESTORE_MESSAGE_DISMISS_TEST=if($TestMessageDismiss){'1'}else{$null}
    $automatedInput=$TestNameEntry -or $TestCreateFamily -or $TestFamilySelection -or $TestFamilyHistory -or $TestFamilyCustomMissions -or $TestFamilyLoadGame -or $TestFamilyDialog -ne 'none' -or $TestHighScores
    $env:RESTORE_INTERNAL_INPUT_ONLY=if($automatedInput){'1'}else{$null}
    if($TestMessageDismiss -and !$TestLoadSaveConfirm){throw 'Message dismissal requires the saved-city confirmation diagnostic'}
    if($TestHighScores -and ($TestNameEntry -or $TestCreateFamily -or $TestFamilySelection -or $TestFamilyHistory -or $TestFamilyCustomMissions -or $TestFamilyLoadGame -or $TestFamilyDialog -ne 'none')){throw 'High scores diagnostic cannot run with a family action diagnostic'}
    if($TestHighScores){$env:RESTORE_MENU_ACTION_TEST='3';$env:RESTORE_MENU_NAME_TEST=$null}
    if($TestNameEntry -or $TestCreateFamily -or $TestFamilySelection -or $TestFamilyHistory -or $TestFamilyCustomMissions -or $TestFamilyLoadGame){$env:RESTORE_MENU_ACTION_TEST='0';$env:RESTORE_MENU_NAME_TEST=if($TestFamilySelection -or $TestFamilyHistory -or $TestFamilyCustomMissions -or $TestFamilyLoadGame){$null}else{'1'}}
    $env:RESTORE_FAMILY_SELECT_TEST=if($TestFamilySelection -or $TestFamilyHistory -or $TestFamilyCustomMissions -or $TestFamilyLoadGame){'1'}else{$null}
    $env:RESTORE_FAMILY_ACTION_TEST=if($TestFamilyHistory){'0'}elseif($TestFamilyCustomMissions){'3'}elseif($TestFamilyLoadGame){'2'}else{$null}
    $env:RESTORE_MISSION_BACK_TEST=if($TestMissionBack){'1'}else{$null}
    $env:RESTORE_LOAD_SAVE_BACK_TEST=if($TestLoadSaveBack){'1'}else{$null}
    $env:RESTORE_LOAD_SAVE_CONFIRM_TEST=if($TestLoadSaveConfirm){'1'}else{$null}
    if($TestLoadSaveConfirm -and (!$TestFamilyLoadGame -or $TestLoadSaveBack)){throw 'Confirm diagnostic requires TestFamilyLoadGame and cannot run with cancel'}
    if($TestLoadSaveBack -and !$TestFamilyLoadGame){throw 'Load/save back diagnostic requires TestFamilyLoadGame'}
    if($TestMissionBack -and !$TestFamilyCustomMissions){throw 'Mission back diagnostic requires TestFamilyCustomMissions'}
    if($TestNonemptyCustomMission -and !$TestFamilyCustomMissions){throw 'Nonempty mission diagnostic requires TestFamilyCustomMissions'}
    if(@($TestFamilyCustomMissions,$TestFamilyHistory,$TestFamilyLoadGame | Where-Object {$_}).Count -gt 1){throw 'Select only one family action diagnostic'}
    if($TestFamilyHistory -or $TestFamilyCustomMissions -or $TestFamilyLoadGame) {
        $familyFixtureRoot=[IO.Path]::GetFullPath((Join-Path $restoreRoot 'build/debug-startup-game'))
        if($AssetDirectory -ne $familyFixtureRoot){throw 'History diagnostic requires the isolated debug-startup-game assets'}
        $familyProfiles=@(Get-ChildItem -LiteralPath (Join-Path $AssetDirectory 'Save') -Filter '*.dat' -File)
        if($familyProfiles.Count -ne 1 -or $familyProfiles[0].Name -ne 'Restore.dat'){throw 'History diagnostic requires its single Restore fixture profile'}
        $saveFiles=@(Get-ChildItem -LiteralPath (Join-Path $AssetDirectory 'Save/Restore') -Force -Filter '*.sav')
        if($TestFamilyLoadGame) {
            $saveProof=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/mission-save-emulation.json') -Raw | ConvertFrom-Json
            $saveFixture=@($saveProof.saves | Where-Object {$_.path -eq 'artifacts/mission-chunks-fixtures/mission-00.sav'})
            if(!$saveProof.passed -or $saveFixture.Count -ne 1 -or $saveFiles.Count -ne 1 -or $saveFiles[0].Name -ne 'restore-native.sav' -or
               (Get-FileHash -LiteralPath $saveFiles[0].FullName -Algorithm SHA256).Hash.ToLowerInvariant() -ne $saveFixture[0].sha256){throw 'Load/save diagnostic requires its verified native SAV fixture'}
        } elseif($saveFiles.Count){throw 'History diagnostic requires an empty fixture save directory'}
        if($TestFamilyDialog -ne 'none' -or $TestCreateFamily){throw 'History diagnostic cannot run with create/delete fixtures'}
        if($TestFamilyCustomMissions) {
            $mapFiles=@(Get-ChildItem -LiteralPath (Join-Path $AssetDirectory 'Maps') -Filter '*.map' -File)
            if($TestNonemptyCustomMission) {
                $mapProof=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/mission-map-emulation.json') -Raw | ConvertFrom-Json
                $mapFixture=@($mapProof.maps | Where-Object {$_.path -eq 'artifacts/mission-map-fixtures/mission-52.map'})
                if(!$mapProof.passed -or $mapFixture.Count -ne 1 -or $mapFiles.Count -ne 1 -or $mapFiles[0].Name -ne 'restore-native-v160.map' -or
                   (Get-FileHash -LiteralPath $mapFiles[0].FullName -Algorithm SHA256).Hash.ToLowerInvariant() -ne $mapFixture[0].sha256){throw 'Nonempty mission diagnostic requires its verified native v160 fixture'}
            } elseif($mapFiles.Count){throw 'Empty custom mission diagnostic requires an empty isolated Maps folder'}
        }
    }
    if($TestCreateFamily) {
        $familyFixtureRoot=[IO.Path]::GetFullPath((Join-Path $restoreRoot 'build/debug-startup-game'))
        if($AssetDirectory -ne $familyFixtureRoot){throw 'Creation diagnostic requires the isolated debug-startup-game assets'}
        if(Test-Path -LiteralPath (Join-Path $AssetDirectory 'Save/Restore.dat')){throw 'Creation diagnostic refuses to overwrite its existing Restore profile'}
        New-Item -ItemType Directory -Path (Join-Path $AssetDirectory 'Save') -Force | Out-Null
        $env:RESTORE_MENU_SUBMIT_TEST='1'
    } else {$env:RESTORE_MENU_SUBMIT_TEST=$null}
    $env:RESTORE_FAMILY_DIALOG_TEST=$null
    if($TestFamilyDialog -ne 'none') {
        $familyFixtureRoot=[IO.Path]::GetFullPath((Join-Path $restoreRoot 'build/debug-startup-game'))
        if($AssetDirectory -ne $familyFixtureRoot){throw 'Dialog diagnostic requires the isolated debug-startup-game assets'}
        $familyProfiles=@(Get-ChildItem -LiteralPath (Join-Path $AssetDirectory 'Save') -Filter '*.dat' -File)
        if($familyProfiles.Count -ne 1 -or $familyProfiles[0].Name -ne 'Restore.dat' -or $familyProfiles[0].Length -ne 0x4a9c){throw 'Dialog diagnostic requires its single Restore fixture profile'}
        if($TestFamilyDialog -eq 'delete-confirm') {
            $familyBaseline=Get-Content -LiteralPath (Join-Path $restoreRoot 'metadata/menu-reconstruction-progress.json') -Raw | ConvertFrom-Json
            if((Get-FileHash -LiteralPath $familyProfiles[0].FullName -Algorithm SHA256).Hash.ToLowerInvariant() -ne $familyBaseline.owned_C_integration.created_fixture_profile.sha256){throw 'Deletion fixture differs from the recorded C-created profile'}
            $familySaveFiles=@(Get-ChildItem -LiteralPath (Join-Path $AssetDirectory 'Save/Restore') -Force)
            if(@($familySaveFiles | Where-Object {$_.Name -notin @('restore-dialog-fixture.sav','restore-dialog-retained.txt') -or $_.PSIsContainer}).Count){throw 'Deletion diagnostic refuses nonfixture family contents'}
        }
        $env:RESTORE_MENU_ACTION_TEST='0';$env:RESTORE_FAMILY_SELECT_TEST=$null
        $env:RESTORE_FAMILY_DIALOG_TEST=switch($TestFamilyDialog){'duplicate'{'1'} 'delete-cancel'{'2'} 'delete-confirm'{'3'}}
        $env:RESTORE_MENU_NAME_TEST=if($TestFamilyDialog -eq 'duplicate'){'1'}else{$null}
        $env:RESTORE_MENU_SUBMIT_TEST=if($TestFamilyDialog -eq 'duplicate'){'1'}else{$null}
    }
    Push-Location -LiteralPath $AssetDirectory
    try {& $debugger $target '--game-startup-probe' *> $Log;$result=$LASTEXITCODE}
    finally {Pop-Location}
} finally {
    $env:RESTORE_DEBUG_TIMEOUT_MS=$savedTimeout
    $env:RESTORE_DEBUG_COMPAT16=$savedCompatibility
    $env:RESTORE_STARTUP_CONTINUE_TEST=$savedContinueTest
    $env:RESTORE_STARTUP_SKIP_VIDEO_TEST=$savedSkipTest
    $env:RESTORE_MENU_ACTION_TEST=$savedMenuActionTest
    $env:RESTORE_MENU_NAME_TEST=$savedMenuNameTest
    $env:RESTORE_MENU_SUBMIT_TEST=$savedMenuSubmitTest
    $env:RESTORE_FAMILY_SELECT_TEST=$savedFamilySelectTest
    $env:RESTORE_FAMILY_DIALOG_TEST=$savedFamilyDialogTest
    $env:RESTORE_FAMILY_ACTION_TEST=$savedFamilyActionTest
    $env:RESTORE_TRACE_STARTUP=$savedTraceStartup
    $env:RESTORE_MISSION_BACK_TEST=$savedMissionBackTest
    $env:RESTORE_LOAD_SAVE_BACK_TEST=$savedLoadSaveBackTest
    $env:RESTORE_LOAD_SAVE_CONFIRM_TEST=$savedLoadSaveConfirmTest
    $env:RESTORE_HIGH_SCORES_TEST=$savedHighScoresTest
    $env:RESTORE_MESSAGE_DISMISS_TEST=$savedMessageDismissTest
    $env:RESTORE_INTERNAL_INPUT_ONLY=$savedInternalInputOnly
}
Write-Host "Bounded standalone C debug log: $Log"
exit $result
