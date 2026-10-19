# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
# Parse Windows bootstrap scripts without executing installers or builds.
$ErrorActionPreference = 'Stop'
$files = @(
    (Join-Path $PSScriptRoot 'setup_windows_build.ps1'),
    (Join-Path $PSScriptRoot 'check_windows_build_syntax.ps1')
)
$failed = $false
foreach ($path in $files) {
    $tokens = $null
    $errors = $null
    [System.Management.Automation.Language.Parser]::ParseFile(
        $path, [ref]$tokens, [ref]$errors
    ) | Out-Null
    if ($errors.Count -gt 0) {
        $failed = $true
        foreach ($parseError in $errors) {
            Write-Host ("[NoeEngine] PowerShell syntax error in " + $path + ":" +
                $parseError.Extent.StartLineNumber + ": " + $parseError.Message) -ForegroundColor Red
        }
    } else {
        Write-Host "[NoeEngine] Parsed successfully: $path"
    }
}
if ($failed) { exit 1 }
exit 0
