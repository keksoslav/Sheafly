# Security Policy

Sheafly's core promise is that **your documents never leave your machine**. Security and privacy are the product, so we take reports seriously.

## Reporting a vulnerability

Please **do not** open a public issue for security problems.

Instead, use GitHub's [private vulnerability reporting](https://docs.github.com/en/code-security/security-advisories/guidance-on-reporting-and-writing-information-about-vulnerabilities/privately-reporting-a-security-vulnerability) ("Report a vulnerability" under the repo's **Security** tab), or email the maintainer.

We aim to acknowledge reports within 72 hours.

## In scope (high priority)

- **Any network egress from the core / offline mode** — if Sheafly sends document contents anywhere without explicit opt-in, that is a critical bug.
- Redaction that does not actually remove underlying text/image data.
- Metadata or hidden data (layers, embedded files, JS) surviving a "scrub" operation.
- Local model / plugin sandbox escapes.

## Supported versions

Until a `1.0` release, only the latest `main` is supported.
