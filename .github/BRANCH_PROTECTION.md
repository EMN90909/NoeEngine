# Protecting main: repository settings required

`CODEOWNERS` is set to `@EMN90909`, but it **does not enforce** review
or branch protection on its own.

### Current limitation

The GitHub rulesets API returned **403**: "Upgrade to GitHub Pro or make this
repository public to enable this feature." The classic protection endpoint
also returned **403 Resource not accessible by integration**. Therefore this
connector cannot activate protection on your behalf, and **main is not
confirmed protected**.

### Configure in GitHub

In repository **Settings → Rules → Rulesets** (if available) or
**Settings → Branches → Add branch protection rule**:

- Target branch `main`.
- Require pull requests before merging; require at least one approval.
- Require CODEOWNERS approval if supported.
- Require conversation resolution; block force pushes and deletions.
- Restrict direct pushes and bypass permissions to trusted maintainers.
- Require passing checks `Packaging and script preflight` and
  `Linux editor (SCons)` only after both genuinely pass on the active
  GitHub runners. Otherwise the branch becomes permanently blocked.
- Consider signed commits and dependency review where supported.

If rulesets are unavailable for the current repository plan or visibility,
review the repository's plan/visibility settings. Changes to repository
visibility should be deliberate, not an automatic workaround.
