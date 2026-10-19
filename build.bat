@echo off
setlocal EnableExtensions DisableDelayedExpansion
rem NoeEngine one-command Windows bootstrap + native editor/installer.
cd /d "%~dp0" || exit /b 1
if not exist "SConstruct" (
  echo ERROR: Complete source checkout required. 1>&2
  exit /b 2
)
if not exist "misc\setup_windows_build.ps1" (
  echo ERROR: Windows build bootstrap missing. 1>&2
  exit /b 2
)
set "JOBS=2"
if not "%~1"=="" set "JOBS=%~1"
echo(%JOBS%| findstr /r "^[1-9][0-9]*$" >nul
if errorlevel 1 (
  echo ERROR: Usage build.bat [jobs] 1>&2
  exit /b 2
)
where powershell.exe >nul 2>&1
if errorlevel 1 (
  echo ERROR: Windows PowerShell 5.1 is required. 1>&2
  exit /b 2
)
powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0misc\setup_windows_build.ps1" -Jobs %JOBS%
if errorlevel 1 (
  echo ERROR: Windows dependency setup or build failed. 1>&2
  exit /b 1
)
exit /b 0
