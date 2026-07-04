# Contributing to Sheafly

Thanks for your interest in Sheafly. This document describes the workflow.

## Branching model

We use a simple trunk-based flow with short-lived feature branches:

- `main` is always releasable and is **protected** — no direct pushes.
- Every change lands via a **pull request** from a feature branch.
- Branch naming: `feat/<slug>`, `fix/<slug>`, `docs/<slug>`, `chore/<slug>`, `refactor/<slug>`.
  - Examples: `feat/merge-split`, `feat/local-ai-chat`, `fix/redaction-metadata`.
- Create feature branches from the latest `main` **when you start the work** — not in advance — so they don't drift.

## Pull requests

1. Branch off `main`.
2. Make focused commits (see below).
3. Open a PR into `main`. Fill in the PR template.
4. CI must pass and conversations must be resolved before merge.
5. Prefer **squash merge** to keep `main` history linear and readable.

## Commit messages

We follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<optional scope>): <short summary>
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`.

Example: `feat(editor): find-and-replace across full document`

## Code of conduct

By participating you agree to the [Code of Conduct](CODE_OF_CONDUCT.md).

## Security

Please report vulnerabilities privately — see [SECURITY.md](SECURITY.md). Given Sheafly's privacy promise, any accidental network egress from the core is treated as a security bug.
