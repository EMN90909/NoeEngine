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
function Get-MingwStatus([string]$MsysRoot) {
    $bin = Join-Path $MsysRoot 'ucrt64\bin'
    # Official MSYS2 UCRT64 packages install both generic and target-prefixed
    # compilers. windres is supplied by the binutils dependency.
    $expected = @('gcc.exe', 'g++.exe', 'windres.exe',
                  'x86_64-w64-mingw32-gcc.exe', 'x86_64-w64-mingw32-g++.exe')
    $missing = @()
    foreach ($exe in $expected) {
        if (-not (Test-Path -LiteralPath (Join-Path $bin $exe) -PathType Leaf)) {
            $missing += $exe
        }
    }
    return [PSCustomObject]@{ Bin = $bin; Missing = @($missing); Ready = ($missing.Count -eq 0) }
}
function Has-Mingw([string]$MsysRoot) {
    if (-not $MsysRoot) { return $false }
    $status = Get-MingwStatus $MsysRoot
    return $status.Ready
}
function Show-MingwDiagnostics([string]$MsysRoot) {
    $status = Get-MingwStatus $MsysRoot
    Write-Host "[NoeEngine] MSYS2 location: $MsysRoot"
    Write-Host "[NoeEngine] Checking compiler directory: $($status.Bin)"
    foreach ($exe in $status.Missing) {
        Write-Host "[NoeEngine] MISSING: $($status.Bin)\$exe" -ForegroundColor Red
    }
    if (Test-Path -LiteralPath $status.Bin) {
        Get-ChildItem -LiteralPath $status.Bin -Filter '*gcc*.exe' -ErrorAction SilentlyContinue |
            Select-Object -First 10 -ExpandProperty Name | ForEach-Object {
                Write-Host "[NoeEngine] Found: $_"
            }
    }
    if ($status.Ready) { Write-Host '[NoeEngine] All GCC, G++ and windres executables exist.' }
}
function Invoke-VisibleNative {
    # PowerShell 5.1 promotes a native program's stderr to NativeCommandError
    # when ErrorActionPreference=Stop. Merge both streams explicitly so GCC,
    # SCons, pacman and windres errors ALWAYS appear in the batch window and
    # Start-Transcript. Never swallow the program's actual exit code.
    param(
        [string]$File,
        [string[]]$ArgumentList,
        [string]$Phase,
        [switch]$AllowCoreRestart
    )
    Write-Host "[NoeEngine] >>> $Phase"
    $previousPreference = $ErrorActionPreference
    $state = @{ CoreRestart = $false }
    $exitStatus = 1
    try {
        $ErrorActionPreference = 'Continue'
        & $File @ArgumentList 2>&1 | ForEach-Object {
            $line = [string]$_
            if ($line -match 'To complete this update all MSYS2 processes') {
                $state.CoreRestart = $true
            }
            if ($line -match '(?i)(fatal error|\berror:|\bfailed\b|Traceback|undefined reference|collect2:)') {
                Write-Host $line -ForegroundColor Red
            } elseif ($line -match '(?i)\bwarning[: ]') {
                Write-Host $line -ForegroundColor Yellow
            } else {
                Write-Host $line
            }
        }
        $exitStatus = $LASTEXITCODE
    } finally {
        $ErrorActionPreference = $previousPreference
    }
    if ($exitStatus -ne 0) {
        if ($AllowCoreRestart -and $state.CoreRestart) {
            # MSYS2 deliberately kills its own bash/pacman after updating
            # msys2-runtime. Only this identified case may continue.
            Write-Host '[NoeEngine] MSYS2 core updated and closed its shell. Starting a fresh shell.' -ForegroundColor Yellow
            return $true
        }
        throw "$Phase failed (exit $exitStatus). The complete errors are printed above and saved in the transcript."
    }
    return $state.CoreRestart
}
function Invoke-Msys([string]$MsysRoot, [string]$Command, [string]$Phase, [switch]$AllowCoreRestart) {
    $env:MSYSTEM = 'UCRT64'
    $env:CHERE_INVOKING = 'yes'
    return Invoke-VisibleNative -File (Join-Path $MsysRoot 'usr\bin\bash.exe') -ArgumentList @('-lc', $Command) -Phase $Phase -AllowCoreRestart:$AllowCoreRestart
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
    # Run package-database health checks before allowing dependency changes.
    # Use MSYS2's cygpath.exe so Windows drive paths and spaces are converted
    # correctly. Do not build an incorrect /c/... path from an arbitrary root.
    $cygpath = Join-Path $msysRoot 'usr\bin\cygpath.exe'
    if (-not (Test-Path -LiteralPath $cygpath)) { throw "MSYS2 cygpath.exe missing from $msysRoot" }
    $repairScript = & $cygpath -u (Join-Path $root 'misc\msys2_pacman_health.sh')
    if ($LASTEXITCODE -ne 0 -or -not $repairScript) { throw 'Failed to resolve MSYS2 Pacman health-check script.' }
    if ($repairScript.Contains("'")) { throw 'Source path containing an apostrophe cannot be safely used by the MSYS2 repair command.' }
    $healthMode = if ($SkipInstall -or $CheckOnly) { '--check' } else { '--repair' }
    Invoke-Msys $msysRoot "bash '$repairScript' $healthMode" 'Inspect local MSYS2 package database' | Out-Null

    if (-not (Has-Mingw $msysRoot)) {
        Show-MingwDiagnostics $msysRoot
        if ($SkipInstall) { throw 'Required MSYS2 GCC/G++/windres files missing. Remove -SkipInstall to reinstall the damaged compiler.' }
        Write-Host '[NoeEngine] Updating MSYS2 in separate, restart-safe shells...'
        $msysUpdated = $false
        for ($pass = 1; $pass -le 4; $pass++) {
            # The explicit y confirms MSYS2's special core-runtime restart prompt.
            $restart = Invoke-Msys $msysRoot "printf 'y\n' | pacman --noconfirm -Syuu" "MSYS2 full update pass $pass" -AllowCoreRestart
            if (-not $restart) {
                $msysUpdated = $true
                break
            }
        }
        if (-not $msysUpdated) { throw 'MSYS2 repeatedly requested a core restart. Restart its UCRT64 terminal and retry.' }
        Invoke-Msys $msysRoot "bash '$repairScript' --repair" 'Verify Pacman metadata after runtime upgrade' | Out-Null

        # IMPORTANT: do NOT use --needed here. A partially installed compiler
        # may be registered in pacman's database despite missing executable
        # files, so --needed silently skips the reinstall and the build fails.
        Write-Host '[NoeEngine] Reinstalling GCC and binutils to restore missing tool files...'
        Invoke-Msys $msysRoot 'pacman --noconfirm -S mingw-w64-ucrt-x86_64-binutils mingw-w64-ucrt-x86_64-gcc' 'Install/repair MSYS2 UCRT64 compiler and resource tools' | Out-Null
    }
    if (-not (Has-Mingw $msysRoot)) {
        Show-MingwDiagnostics $msysRoot
        Write-Host '[NoeEngine] Checking Pacman package installation records...'
        Invoke-Msys $msysRoot 'pacman -Q mingw-w64-ucrt-x86_64-binutils mingw-w64-ucrt-x86_64-gcc' 'Show installed GCC/binutils package versions' | Out-Null
        throw 'GCC package records exist but required executables are missing. Check the exact missing paths above, MSYS2 root location, and full transcript. The batch window will stay open.'
    }
    $gccBin = Join-Path $msysRoot 'ucrt64\bin'
    $env:PATH = "$gccBin;$env:PATH"
    $env:MSYSTEM = 'UCRT64'
    # MSYS2's POSIX prefix (/ucrt64) is not a usable filesystem path in a
    # native Windows Python/SCons process. SCons explicitly gets
    # mingw_prefix=; clear the inherited prefix for other subprocesses.
    $env:MINGW_PREFIX = ''
    Write-Host "[NoeEngine] Compiler: $gccBin"
    Invoke-VisibleNative -File (Join-Path $gccBin 'x86_64-w64-mingw32-gcc.exe') -ArgumentList @('--version') -Phase 'Verify GCC compiler' | Out-Null
    Invoke-VisibleNative -File (Join-Path $gccBin 'x86_64-w64-mingw32-g++.exe') -ArgumentList @('--version') -Phase 'Verify G++ compiler' | Out-Null
    Invoke-VisibleNative -File (Join-Path $gccBin 'windres.exe') -ArgumentList @('--version') -Phase 'Verify resource compiler' | Out-Null

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
    Invoke-VisibleNative -File $python.Path -ArgumentList (@($python.Args) + @('-u', 'misc/build_products.py', 'windows', '--preflight')) -Phase 'Check Windows GCC, windres and NSIS prerequisites' | Out-Null
    if ($CheckOnly) {
        Write-Host '[NoeEngine] PASS: MinGW-w64 preflight (no source compiled).'
    } else {
        Write-Host "[NoeEngine] Building Windows x64 editor with $Jobs GCC job(s)..."
        Write-Host '[NoeEngine] --- Compiler messages will stream below in real time ---'
        Invoke-VisibleNative -File $python.Path -ArgumentList (@($python.Args) + @('-u', 'misc/build_products.py', 'windows', '--jobs', [string]$Jobs)) -Phase 'SCons MinGW-w64 compilation and NSIS packaging' | Out-Null
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
