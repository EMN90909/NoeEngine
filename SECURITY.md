# NoeEngine security policy

## Scope and trust boundaries

NoeEngine is an early fork of Godot 4.0.5 RC. Game projects, addons, scripts,
native extensions, importers, downloaded assets, exporters, and CI dependencies
can execute untrusted code or parse untrusted files. Only open untrusted
projects in an appropriately isolated environment.

## Build security

- CI workflows should request only `contents: read`; never expose long-lived
  personal access tokens to forks or third-party actions.
- GitHub-owned actions have been pinned to verified commit SHAs in the
  actively used workflows. Third-party actions must be separately reviewed
  and SHA-pinned before enabling optional legacy workflows.
- Generated `Products/` binaries are ignored by Git and should be scanned,
  signed, and verified before distribution.
- Releases require upstream MIT copyright and third-party attribution.
- Preserve original Git history and do not remove original copyright notices.
- Do not place secrets in `custom.py`, source code, YAML, logs, or artifacts.
- Do not disable dependency checks to get green CI.

## Reports and remediation

Open a private vulnerability report using GitHub's private reporting feature
if available for the repository; otherwise contact repository maintainers
privately through an established channel. Do not post proof-of-concept
exploit details or secret values in public issues.

This document describes policy, not a completed penetration test or a
certification that the old upstream engine has no known vulnerabilities.
