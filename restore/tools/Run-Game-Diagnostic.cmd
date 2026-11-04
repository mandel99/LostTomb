@echo off
setlocal
for %%I in ("%~dp0..\build\c-only\Release") do set "RESTORE_RUN_DIR=%%~fI"
pushd "%RESTORE_RUN_DIR%" || exit /b 2
if not exist "restore_debug.exe" goto missing
if not exist "PharaohRestoreC.exe" goto missing
set "RESTORE_DEBUG_TIMEOUT_MS=300000"
set "RESTORE_DEBUG_SNAPSHOT_MS=10000"
set "RESTORE_DEBUG_COMPAT16=1"
set "RESTORE_DEBUG_READ_AT=d056f8,78d7cc,78d790,eaac28,605a3c,e38dd6,78d838,78d588"
set "RESTORE_DEBUG_BREAK_AT="
if exist "PharaohRestoreC.map" for /f "tokens=3" %%A in ('findstr /c:" _abort " "PharaohRestoreC.map"') do set "RESTORE_DEBUG_BREAK_AT=%%A"
echo Load the first mission and reproduce the crash. This diagnostic run is limited to five minutes.
echo Output: "%RESTORE_RUN_DIR%\PharaohRestoreC-diagnostic.log"
echo If the game freezes, wait at least 20 seconds before closing it. Stack snapshots are saved every 10 seconds.
>>"PharaohRestoreC-diagnostic.log" echo ==== RUN %DATE% %TIME% ====
>>"PharaohRestoreC-diagnostic.log" echo EXE="%RESTORE_RUN_DIR%\PharaohRestoreC.exe"
>>"PharaohRestoreC-diagnostic.log" echo ABORT_BREAKPOINT=%RESTORE_DEBUG_BREAK_AT%
"%SystemRoot%\System32\certutil.exe" -hashfile "PharaohRestoreC.exe" SHA256 >>"PharaohRestoreC-diagnostic.log" 2>&1
"restore_debug.exe" "%RESTORE_RUN_DIR%\PharaohRestoreC.exe" >>"PharaohRestoreC-diagnostic.log" 2>&1
set "RESTORE_DEBUG_RESULT=%ERRORLEVEL%"
>>"PharaohRestoreC-diagnostic.log" echo DIAGNOSTIC_RESULT=%RESTORE_DEBUG_RESULT% %DATE% %TIME%
echo Diagnostic finished. The log contains the executable identity, exceptions and abort stack.
pause
popd
exit /b %RESTORE_DEBUG_RESULT%
:missing
echo Missing PharaohRestoreC.exe or restore_debug.exe in "%RESTORE_RUN_DIR%".
pause
popd
exit /b 2
