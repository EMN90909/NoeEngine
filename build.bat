@echo off
setlocal EnableExtensions DisableDelayedExpansion
rem NoeEngine native Windows x64 editor build and installer.
rem Requires Python 3, SCons, and Visual Studio C++ Build Tools or MinGW-w64.
rem Uses the same packaging implementation as build.sh.
cd /d "%~dp0" || exit /b 1
if not exist "Products" mkdir "Products"
if not exist "SConstruct" (
  echo ERROR: SConstruct missing. Run from a complete source checkout. 1>&2
  exit /b 2
)
if not exist "misc\build_products.py" (
  echo ERROR: misc\build_products.py missing. 1>&2
  exit /b 2
)
set "PYTHON=python"
where python >nul 2>&1
if errorlevel 1 (
  set "PYTHON=py -3"
  where py >nul 2>&1
  if errorlevel 1 (
    echo ERROR: Python 3 is required. 1>&2
    exit /b 2
  )
)
set "JOBS=2"
if not "%~1"=="" set "JOBS=%~1"
echo(%JOBS%| findstr /r "^[1-9][0-9]*$" >nul
if errorlevel 1 (
  echo ERROR: jobs must be a positive integer. Usage: build.bat [jobs] 1>&2
  exit /b 2
)
echo [NoeEngine] Checking Windows build toolchain...
%PYTHON% misc\build_products.py windows --preflight
if errorlevel 1 exit /b 1
echo [NoeEngine] Building native Windows editor and installer...
%PYTHON% misc\build_products.py windows --jobs %JOBS%
if errorlevel 1 exit /b 1
if not exist "Products\NoeEngine-setup.exe" (
  echo ERROR: Windows installer was not produced. 1>&2
  exit /b 1
)
for %%F in ("Products\NoeEngine-setup.exe") do if %%~zF LEQ 0 (
  echo ERROR: Windows installer is empty. 1>&2
  exit /b 1
)
echo [NoeEngine] SUCCESS: Products\NoeEngine-setup.exe
exit /b 0
