# NoeEngine native Windows MinGW-w64 GCC bootstrap (no Visual Studio / Windows SDK).
# Called by build.bat. Missing tools come from WinGet and official MSYS2 pacman.
[CmdletBinding()]
param([ValidateRange(1, 64)][int]$Jobs = 2, [switch]$SkipInstall, [switch]$CheckOnly)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $root
$logFolder = Join-Path $root 'Products\logs'
New-Item -ItemType Directory -Force -Path $logFolder | Out-Null
$logFile = Join-Path $logFolder ("windows-mingw-{0}-{1}.log" -f (Get-Date -Format 'yyyyMMdd-HHmmss'), $PID)
$recording = $false
$exitCode = 0

function Refresh-ToolPath {
    $machine = [Environment]::GetEnvironmentVariable('PATH', 'Machine')
    $user = [Environment]::GetEnvironmentVariable('PATH', 'User')
    # Keep caller's PATH first (e.g., actions/setup-python; MSYS2 UCRT64).
    if ($machine -or $user) { $env:PATH = "$env:PATH;$machine;$user" }
    $pf86 = [Environment]::GetEnvironmentVariable('ProgramFiles(x86)')
    $nsisPaths = @("$env:ProgramFiles\NSIS")
    if ($pf86) { $nsisPaths += (Join-Path $pf86 'NSIS') }
    foreach ($dir in $nsisPaths) {
        if ((Test-Path $dir) -and ($env:PATH -notlike "*$dir*")) {
            $env:PATH = "$dir;$env:PATH"
        }
    }
}
function Get-Python {
    foreach ($exe in @('python.exe', 'py.exe')) {
        $command = Get-Command $exe -ErrorAction SilentlyContinue
        if (-not $command) { continue }
        $args = if ($exe -eq 'py.exe') { @('-3') } else { @() }
        try {
            & $command.Source @args -c 'import sys; assert sys.version_info >= (3, 9)' 2>$null
            if ($LASTEXITCODE -eq 0) { return @{ Path = $command.Source; Args = $args } }
        } catch {}
    }
    return $null
}
function Install-Package([string]$Id) {
    if ($SkipInstall) { throw "Missing $Id. Run build.bat to allow installation." }
    if (-not (Get-Command winget.exe -ErrorAction SilentlyContinue)) {
        throw 'WinGet is missing. Install Microsoft App Installer, or install MSYS2 UCRT64, Python, and NSIS manually.'
    }
    Write-Host "[NoeEngine] Installing $Id via WinGet..."
    & winget.exe install --id $Id --exact --source winget --silent --accept-package-agreements --accept-source-agreements
    if ($LASTEXITCODE -ne 0) { throw "WinGet install failed for $Id (exit $LASTEXITCODE)." }
    Refresh-ToolPath
}
function Find-MsysRoot {
    $roots = @()
    if ($env:NOEENGINE_MSYS2_ROOT) { $roots += $env:NOEENGINE_MSYS2_ROOT }
    $roots += 'C:\msys64'
    if ($env:LOCALAPPDATA) { $roots += (Join-Path $env:LOCALAPPDATA 'Programs\msys64') }
    foreach ($candidate in $roots) {
        if (Test-Path (Join-Path $candidate 'usr\bin\bash.exe')) {
            return (Resolve-Path $candidate).Path
        }
    }
    return $null
}
function Has-Mingw([string]$MsysRoot) {
    if (-not $MsysRoot) { return $false }
    $bin = Join-Path $MsysRoot 'ucrt64\bin'
    foreach ($exe in @('x86_64-w64-mingw32-gcc.exe', 'x86_64-w64-mingw32-g++.exe', 'x86_64-w64-mingw32-windres.exe')) {
        if (-not (Test-Path (Join-Path $bin $exe))) { return $false }
    }
    return $true
}
function Invoke-Msys([string]$MsysRoot, [string]$Command) {
    $env:MSYSTEM = 'UCRT64'
    $env:CHERE_INVOKING = 'yes'
    & (Join-Path $MsysRoot 'usr\bin\bash.exe') -lc $Command
    if ($LASTEXITCODE -ne 0) {
        throw "MSYS2 failed (exit $LASTEXITCODE): $Command. Open MSYS2 UCRT64 and review pacman messages above."
    }
}

try {
    Start-Transcript -Path $logFile -Force -ErrorAction Stop | Out-Null
    $recording = $true
    Write-Host "[NoeEngine] Full compiler log: $logFile"
    Write-Host '[NoeEngine] Toolchain: GCC/MinGW-w64 UCRT64 (NO Visual Studio).'
    Refresh-ToolPath

    $python = Get-Python
    if (-not $python) {
        Install-Package 'Python.Python.3.12'
        $python = Get-Python
    }
    if (-not $python) { throw 'Python 3.9+ was not found. Open a new terminal or add Python to PATH.' }
    Write-Host "[NoeEngine] Python: $($python.Path)"

    $msysRoot = Find-MsysRoot
    if (-not $msysRoot) {
        Install-Package 'MSYS2.MSYS2'
        $msysRoot = Find-MsysRoot
    }
    if (-not $msysRoot) {
        throw 'MSYS2 is missing. Install to C:\msys64 or set NOEENGINE_MSYS2_ROOT.'
    }
    if (-not (Has-Mingw $msysRoot)) {
        if ($SkipInstall) { throw 'MSYS2 UCRT64 GCC/G++/windres missing. Run build.bat to install them.' }
        Write-Host '[NoeEngine] Updating MSYS2 core packages...'
        Invoke-Msys $msysRoot 'pacman --noconfirm -Syuu'
        Invoke-Msys $msysRoot 'pacman --noconfirm -Syuu'
        Write-Host '[NoeEngine] Installing lightweight GCC/MinGW-w64 toolchain...'
        Invoke-Msys $msysRoot 'pacman --noconfirm --needed -S mingw-w64-ucrt-x86_64-gcc'
    }
    if (-not (Has-Mingw $msysRoot)) {
        throw 'MinGW-w64 compiler still missing. Open MSYS2 UCRT64 and install mingw-w64-ucrt-x86_64-gcc.'
    }
    $gccBin = Join-Path $msysRoot 'ucrt64\bin'
    $env:PATH = "$gccBin;$env:PATH"
    $env:MSYSTEM = 'UCRT64'
    Write-Host "[NoeEngine] Compiler: $gccBin"
    & (Join-Path $gccBin 'x86_64-w64-mingw32-gcc.exe') --version
    if ($LASTEXITCODE -ne 0) { throw 'GCC could not start.' }

    if (-not (Get-Command makensis.exe -ErrorAction SilentlyContinue)) { Install-Package 'NSIS.NSIS' }
    Refresh-ToolPath
    if (-not (Get-Command makensis.exe -ErrorAction SilentlyContinue)) { throw 'NSIS makensis.exe is missing from PATH.' }

    & $python.Path @($python.Args) -m pip --version
    if ($LASTEXITCODE -ne 0) { throw 'pip missing in selected Python.' }
    $sconsAvailable = $false
    try {
        & $python.Path @($python.Args) -c 'import SCons; print(SCons.__version__)' 2>$null | Out-Null
        $sconsAvailable = ($LASTEXITCODE -eq 0)
    } catch { $sconsAvailable = $false }
    if (-not $sconsAvailable) {
        if ($SkipInstall) { throw 'SCons missing. Run build.bat without -SkipInstall.' }
        & $python.Path @($python.Args) -m pip install --user 'scons==4.8.1'
        if ($LASTEXITCODE -ne 0) { throw 'SCons pip installation failed.' }
    }
    foreach ($scheme in @('nt_user', 'nt')) {
        $scripts = & $python.Path @($python.Args) -c "import sysconfig; print(sysconfig.get_path('scripts', '$scheme'))"
        if ($LASTEXITCODE -eq 0 -and $scripts -and (Test-Path $scripts)) {
            $env:PATH = "$scripts;$env:PATH"
        }
    }
    if (-not (Get-Command scons.exe -ErrorAction SilentlyContinue)) {
        throw 'scons.exe missing from PATH. Check the selected Python Scripts folder.'
    }

    Write-Host '[NoeEngine] Checking MinGW-w64 Windows toolchain...'
    & $python.Path @($python.Args) misc/build_products.py windows --preflight
    if ($LASTEXITCODE -ne 0) { throw 'Windows MinGW preflight failed.' }
    if ($CheckOnly) {
        Write-Host '[NoeEngine] PASS: MinGW-w64 preflight (no source compiled).'
    } else {
        Write-Host "[NoeEngine] Building Windows x64 editor with $Jobs GCC job(s)..."
        & $python.Path @($python.Args) misc/build_products.py windows --jobs $Jobs
        if ($LASTEXITCODE -ne 0) { throw "Windows compilation/packaging failed (exit $LASTEXITCODE)." }
        $installer = Join-Path $root 'Products\NoeEngine-setup.exe'
        if (-not (Test-Path $installer) -or (Get-Item $installer).Length -lt 100000) {
            throw 'NoeEngine-setup.exe missing or too small.'
        }
        Write-Host "[NoeEngine] SUCCESS: $installer"
    }
} catch {
    $exitCode = 1
    Write-Host ''
    Write-Host '[NoeEngine] BUILD FAILED:' -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    if ($_.ScriptStackTrace) { Write-Host $_.ScriptStackTrace }
} finally {
    if ($recording) {
        Write-Host "[NoeEngine] Review transcript: $logFile"
        Stop-Transcript -ErrorAction SilentlyContinue | Out-Null
    }
}
exit $exitCode
