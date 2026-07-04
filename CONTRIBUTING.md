# Contributing

## Branches

`main` is protected, no direct pushes. Everything lands through a PR from a branch.

Name branches `feat/x`, `fix/x`, `docs/x`, `chore/x`, or `refactor/x`. Cut them from the latest `main` when you start the work, not before.

## Pull requests

Open against `main`, fill in the template, wait for CI, then squash merge. Branches auto-delete after merge.

## Commits

[Conventional Commits](https://www.conventionalcommits.org/): `feat`, `fix`, `docs`, `refactor`, `chore`, `test`, `ci`, `build`, `perf`, `style`.

Example: `feat(editor): find and replace across the whole document`

## Security

Don't open public issues for security bugs, see [SECURITY.md](SECURITY.md). Any network traffic leaving the offline core counts as a security bug.
