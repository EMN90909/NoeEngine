# NoeEngine Windows native build installer/bootstrap.
# Called locally from build.bat. Uses WinGet trusted manifests, not remote scripts.
[CmdletBinding()]
param([ValidateRange(1,64)][int]$Jobs=2, [switch]$SkipInstall, [switch]$CheckOnly)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $root

function Refresh-ToolPath {
    $machine = [Environment]::GetEnvironmentVariable('PATH', 'Machine')
    $user = [Environment]::GetEnvironmentVariable('PATH', 'User')
    if ($machine -or $user) { $env:PATH = "$machine;$user;$env:PATH" }
    foreach ($nsis in @("${env:ProgramFiles(x86)}\NSIS", "$env:ProgramFiles\NSIS")) {
        if (Test-Path $nsis) { $env:PATH = "$nsis;$env:PATH" }
    }
}
function Get-Python {
    foreach ($exe in @('python.exe','py.exe')) {
        $command = Get-Command $exe -ErrorAction SilentlyContinue
        if (-not $command) { continue }
        $args = if ($exe -eq 'py.exe') { @('-3') } else { @() }
        try {
            & $command.Source @args -c 'import sys; assert sys.version_info >= (3, 9)' 2>$null
            if ($LASTEXITCODE -eq 0) { return @{ Path=$command.Source; Args=$args } }
        } catch { }
    }
    return $null
}
function Install-Package([string]$Id, [string]$Override='') {
    if ($SkipInstall) { throw "Missing $Id. Rerun build.bat to install the toolchain." }
    if (-not (Get-Command winget.exe -ErrorAction SilentlyContinue)) {
        throw 'WinGet is required to bootstrap safely. Install Microsoft App Installer, or install Python 3.12, VS 2022 C++ Build Tools and NSIS manually.'
    }
    Write-Host "[NoeEngine] Installing $Id through WinGet..."
    $options=@('install','--id',$Id,'--exact','--accept-package-agreements','--accept-source-agreements','--source','winget')
    if ($Override) { $options += @('--override',$Override) }
    & winget.exe @options
    if ($LASTEXITCODE -ne 0) { throw "WinGet installation of $Id failed (exit $LASTEXITCODE)." }
    Refresh-ToolPath
}
function Find-CppTools {
    $vswhere = Join-Path "${env:ProgramFiles(x86)}" 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path $vswhere)) { return $false }
    $install = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ($LASTEXITCODE -ne 0 -or -not $install) { return $false }
    $sdk = Get-ChildItem 'HKLM:\SOFTWARE\Microsoft\Windows Kits\Installed Roots' -ErrorAction SilentlyContinue
    if (-not $sdk) { throw 'MSVC exists but Windows SDK was not found. Modify Build Tools to add a Windows 10/11 SDK.' }
    return $true
}
Refresh-ToolPath
$python=Get-Python
if (-not $python) {
    Install-Package 'Python.Python.3.12'
    $python=Get-Python
}
if (-not $python) { throw 'Python installation completed but Python was not found. Restart the shell or add Python to PATH.' }
if (-not (Find-CppTools)) {
    Install-Package 'Microsoft.VisualStudio.2022.BuildTools' '--quiet --wait --norestart --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended'
}
if (-not (Find-CppTools)) { throw 'MSVC/Windows SDK missing: enable Desktop C++ tools and Windows SDK in Visual Studio Installer.' }
if (-not (Get-Command makensis.exe -ErrorAction SilentlyContinue)) {
    Install-Package 'NSIS.NSIS'
}
Refresh-ToolPath
if (-not (Get-Command makensis.exe -ErrorAction SilentlyContinue)) { throw 'NSIS installed but makensis.exe not on PATH.' }
# SCons is a Python dependency; use module execution for reliable version checks.
& $python.Path @($python.Args) -m pip --version
if ($LASTEXITCODE -ne 0) { throw 'pip missing from the selected Python installation.' }
$check = & $python.Path @($python.Args) -c 'import SCons; print(SCons.__version__)' 2>$null
if ($LASTEXITCODE -ne 0 -or -not $check) {
    if ($SkipInstall) { throw 'SCons not installed; run build.bat to install dependencies.' }
    & $python.Path @($python.Args) -m pip install --user 'scons==4.8.1'
    if ($LASTEXITCODE -ne 0) { throw 'Failed to install SCons.' }
}
$scriptDir = & $python.Path @($python.Args) -c 'import sysconfig; print(sysconfig.get_path("scripts", "nt_user"))'
if ($LASTEXITCODE -eq 0 -and $scriptDir) { $env:PATH = "$scriptDir;$env:PATH" }
if (-not (Get-Command scons.exe -ErrorAction SilentlyContinue)) {
    $candidate=& $python.Path @($python.Args) -c 'import sysconfig; print(sysconfig.get_path("scripts"))'
    if ($candidate) { $env:PATH="$candidate;$env:PATH" }
}
if (-not (Get-Command scons.exe -ErrorAction SilentlyContinue)) {
    throw 'SCons installed but executable not found. Add the Python Scripts directory to PATH.'
}
Write-Host '[NoeEngine] Checking complete native Windows compiler, SDK and NSIS toolchain...'
& $python.Path @($python.Args) misc/build_products.py windows --preflight
if ($LASTEXITCODE -ne 0) { throw 'NoeEngine Windows preflight failed.' }
if ($CheckOnly) { Write-Host '[NoeEngine] Windows dependency preflight OK'; exit 0 }
New-Item -ItemType Directory -Force -Path (Join-Path $root 'Products') | Out-Null
Write-Host '[NoeEngine] Compiling Windows x64 editor and creating installer...'
& $python.Path @($python.Args) misc/build_products.py windows --jobs $Jobs
if ($LASTEXITCODE -ne 0) { throw "NoeEngine native Windows build failed (exit $LASTEXITCODE)." }
$installer = Join-Path $root 'Products\NoeEngine-setup.exe'
if (-not (Test-Path $installer) -or (Get-Item $installer).Length -lt 100000) {
    throw 'A nonempty, valid-sized NoeEngine-setup.exe was not generated.'
}
Write-Host "[NoeEngine] SUCCESS: $installer"
