@echo off
setlocal
if not "%~1"=="" goto command
if exist "%~dp0artifacts\ollama-batch\plan.json" goto run
python "%~dp0tools\ollama_batch.py" plan
if errorlevel 1 exit /b 1
:run
python "%~dp0tools\ollama_batch.py" run
exit /b %errorlevel%
:command
python "%~dp0tools\ollama_batch.py" %*
exit /b %errorlevel%
