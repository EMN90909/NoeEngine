@echo off
setlocal EnableExtensions DisableDelayedExpansion
rem NoeEngine Windows x64 native compiler: MSYS2 UCRT64 GCC, never Visual Studio.
rem On any error keep the console open; set NOEENGINE_NO_PAUSE=1 for CI.
cd /d "%~dp0"
if errorlevel 1 (
  echo [NoeEngine] ERROR: Cannot enter the source directory.
  set "RESULT=2"
  goto :failed
)
if not exist "SConstruct" (
  echo [NoeEngine] ERROR: SConstruct missing. Use a complete source checkout.
  set "RESULT=2"
  goto :failed
)
if not exist "misc\setup_windows_build.ps1" (
  echo [NoeEngine] ERROR: misc\setup_windows_build.ps1 is missing.
  set "RESULT=2"
  goto :failed
)
set "JOBS=2"
if not "%~1"=="" set "JOBS=%~1"
echo(%JOBS%| findstr /r "^[1-9][0-9]*$" >nul
if errorlevel 1 (
  echo [NoeEngine] ERROR: Usage: build.bat [jobs]
  set "RESULT=2"
  goto :failed
)
where powershell.exe >nul 2>&1
if errorlevel 1 (
  echo [NoeEngine] ERROR: Windows PowerShell is required.
  set "RESULT=2"
  goto :failed
)
echo.
echo [NoeEngine] Using MSYS2 MinGW-w64 GCC, not Visual Studio.
echo [NoeEngine] All compiler output stays visible; transcript: Products\logs\
echo.
powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0misc\setup_windows_build.ps1" -Jobs %JOBS%
set "RESULT=%ERRORLEVEL%"
if not "%RESULT%"=="0" goto :failed
echo.
echo [NoeEngine] SUCCESS: Products\NoeEngine-setup.exe
exit /b 0

:failed
echo.
echo ============================================================
echo [NoeEngine] BUILD FAILED - Error code %RESULT%.
echo [NoeEngine] Check the errors above and Products\logs\
echo ============================================================
echo.
rem Pause by default so the window does not vanish after double-clicking.
if /I not "%NOEENGINE_NO_PAUSE%"=="1" pause
exit /b %RESULT%
