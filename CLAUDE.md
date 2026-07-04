# CLAUDE.md

Guidance for Claude Code (and humans) working in the Sheafly repository. This file is the single source of truth for what Sheafly is, how it is built, and the rules that must not be broken. Read it before making changes.

---

## 1. What Sheafly is

Sheafly is a **privacy-first, offline desktop PDF suite**. It matches the feature set of tools like PDF24, then goes past them on three fronts: guaranteed offline privacy, true in-place editing, and opt-in on-device AI.

**The wedge:** every AI-enabled PDF tool on the market uploads your document to a server. Sheafly never does. A lawyer, doctor, or accountant can run summarization, extraction, and chat over a confidential document and be certain nothing left their machine. That guarantee is the product. Everything in this repo serves it.

One-line pitch: *Your documents, sharpened. Fully offline.*

---

## 2. Non-negotiable principles (invariants)

These are hard rules. A change that violates one is a bug, no matter how useful.

1. **The core makes zero network calls.** Document processing, editing, OCR, redaction, and local AI inference must all work with the network cable unplugged. Any outbound connection is opt-in, off by default, clearly labeled in the UI, and never carries document contents unless the user explicitly asked for that specific action.
2. **Redaction actually destroys data.** When content is redacted, the underlying text, vector, and image data is removed from the output file, not just painted over. Every redaction operation must be verifiable.
3. **No hidden telemetry, ever.** No analytics, crash phone-home, or usage pings without an explicit, off-by-default opt-in.
4. **Destructive operations are reversible or confirmed.** Editing never silently overwrites the user's original unless they chose to.
5. **Local-first data.** Documents, settings, and model weights live on the user's disk. No cloud account is required to use any core feature.

If you are unsure whether a change touches one of these, assume it does and flag it in the PR.

---

## 3. The three pillars (product spec)

### Pillar A - Privacy / offline
- Guaranteed zero-network core, with a visible offline indicator and an optional hard network kill-switch.
- **Metadata scrubber:** strip author, producer, creation/modification timestamps, GPS, and edit history in one action.
- **Verifiable redaction:** remove underlying data and emit a proof report listing what was removed and where.
- **Hidden-data audit:** warn before sharing about layers, embedded files, embedded JavaScript, annotations, off-page content, and tracked changes.
- **Local encrypted vault:** optional passphrase-protected store for sensitive PDFs.

### Pillar B - True editing
- Real text editing with reflow and font matching, not annotation overlays.
- **Find-and-replace across the whole document.**
- Vector and image editing in place: crop, replace, move, delete.
- **Visual and textual diff/compare** between two document versions.

### Pillar C - Local AI (opt-in)
- A pluggable inference backend: either a bundled small model or a user-supplied local endpoint (for example a local Ollama server). No cloud provider is wired into the core.
- **Chat-with-PDF** with page-cited answers (every claim links back to a page/region).
- Summarize a document or a selection.
- **Table and invoice extraction** to clean CSV/JSON.
- Auto-generate bookmarks and a table of contents from detected headings.
- Layout-preserving translation.
- Smart rename from content.
- Alt-text generation for images (accessibility).

Classical, non-LLM smart features (OCR, layout analysis, table detection) are always-on and ship in the core. The LLM is an optional engine layered on top, so the base app stays small and fully functional without it.

---

## 4. Feature specification (parity + beyond)

Detailed, living plan lives in [`docs/ROADMAP.md`](docs/ROADMAP.md). Phases:

- **Phase 0 - Foundations:** engine + stack decision, core document model, renderer, CI, packaging, offline-friendly auto-update.
- **Phase 1 - PDF24 parity:** merge, split, extract/reorder/rotate/delete pages, compress, flatten, convert to/from Office and images and HTML and text, annotate, form fill, password protect/unlock, redact, sign, OCR, watermark, page numbers, extract images, compare.
- **Phase 2 - Privacy/offline pillar** (see Pillar A).
- **Phase 3 - True editing pillar** (see Pillar B).
- **Phase 4 - Local AI pillar** (see Pillar C).
- **Phase 5 - Power users:** batch recipes, watch-folders, CLI, data merge, form builder, PDF/A and PDF/UA compliance.

Each feature ships behind its own PR with, at minimum: a short spec in the PR body, tests where feasible, and an explicit check that the offline invariant still holds.

---

## 5. Architecture (target)

Sheafly is designed as a thin UI over a **pure, engine-agnostic core**. The core knows nothing about the network or the UI framework.

```
+--------------------------------------------------+
|                    UI layer                      |  (desktop shell, views, dialogs)
+--------------------------------------------------+
|                 Application core                 |  (pure logic, no network, no UI)
|  - Document model (pages, objects, text, fonts)  |
|  - Operations (merge, split, redact, edit, ...)  |
|  - Offline guard (asserts no egress in core)     |
+--------------------------------------------------+
|   PDF engine     |   OCR     |   AI engine (opt) |  (pluggable adapters)
|  render/parse    |  local    |  local model or   |
|  /write          |           |  user endpoint    |
+--------------------------------------------------+
```

Key design rules:
- **Core is pure and offline.** No socket, HTTP client, or DNS lookup is reachable from the core module graph. Enforced by tests and, where the toolchain allows, by build-time boundaries.
- **Engines are adapters behind interfaces.** The PDF engine, OCR engine, and AI engine each sit behind a narrow trait/interface so they can be swapped or mocked. This keeps the make-or-break engine choice from leaking through the whole codebase.
- **The AI engine is optional and sandboxed.** If no model is configured, every AI feature degrades gracefully to disabled. Model inference runs in a constrained process with no network and no filesystem access beyond what it is handed.

---

## 6. Tech stack - DECISION PENDING (Phase 0)

This is the highest-leverage open decision. Do not scatter a stack choice through the code before it is made; put it here first.

**App shell (leading candidates):**
- **Tauri (Rust core + web UI):** tiny binaries, strong sandboxing, Rust core fits the offline-purity and performance goals and the maintainer's systems background. Web UI is flexible.
- **Qt (C++):** mature native desktop, excellent PDF tooling ecosystem, single-language stack, plays to the maintainer's C++ experience.

**PDF engine (make-or-break, especially for the true-editing pillar):**
- **PDFium** - best-in-class rendering (Chrome's engine), permissive license, weaker at authoring/editing.
- **MuPDF** - strong render + edit, but **AGPL**: linking it likely forces the app to AGPL. Weigh against the MIT license before adopting.
- **Apryse / commercial SDK** - true editing out of the box, but paid and heavy.

**Decision criteria:** true in-place text editing feasibility, license compatibility with MIT, binary size, offline suitability, and maintainer velocity. Record the final decision in this section and in an ADR under `docs/adr/` once made.

---

## 7. Repository layout (target)

```
Sheafly/
  README.md
  LICENSE                 MIT
  CLAUDE.md               this file
  CONTRIBUTING.md         workflow + conventions
  SECURITY.md             vuln reporting + in-scope threats
  CODE_OF_CONDUCT.md
  .github/                CODEOWNERS, PR/issue templates, CI workflows
  docs/
    ROADMAP.md            phased feature plan
    adr/                  architecture decision records (added as decisions land)
  src/                    application code (populated in Phase 0)
  tests/                  test suites (populated in Phase 0)
```

`src/` and `tests/` are intentionally empty until the Phase 0 stack decision lands.

---

## 8. Development workflow

`main` is protected. Nothing is pushed to it directly.

1. Branch off the latest `main`. Names: `feat/<slug>`, `fix/<slug>`, `docs/<slug>`, `chore/<slug>`, `refactor/<slug>`.
2. Make focused commits using **Conventional Commits** (`feat:`, `fix:`, `docs:`, `refactor:`, `chore:`, `test:`, `ci:`, `build:`, `perf:`, `style:`).
3. Open a PR into `main`. Fill in the template.
4. CI (`ci` check) must pass, and all conversations must be resolved.
5. **Squash-merge** only. The branch auto-deletes on merge.

Branch protection currently enforces: PR required, `ci` status check required and up to date, linear history, no force-push, no deletion, conversation resolution required, 0 required approvals (solo maintainer). Feature branches are created **on demand** when work starts, never pre-created empty.

---

## 9. Build, test, and run

To be populated in Phase 0 once the stack lands. Until then there is no build step; CI only runs repository-hygiene checks. When you add build tooling, document the exact commands here (install, build, test, lint, run) so a fresh session can act without guessing.

---

## 10. Coding conventions

- Keep the **core pure**: no UI or network imports in core modules.
- Program to the engine interfaces, not concrete engines.
- Prefer small, single-purpose modules mirroring the operation list (one operation, one module where practical).
- Match the surrounding code's style, naming, and comment density.
- Every user-facing operation has a corresponding headless/core entry point, so features are testable and scriptable (this also feeds the future CLI).
- Line endings are normalized to LF via `.gitattributes`. Do not fight it.

---

## 11. Security and privacy requirements

- Treat any accidental network egress from the core as a **security bug**, not a feature bug. See [`SECURITY.md`](SECURITY.md).
- Redaction, metadata scrubbing, and hidden-data audit each need regression tests proving the data is actually gone from the output bytes.
- Never log document contents. Never write document contents outside paths the user chose.
- Model weights and caches are gitignored and never committed.
- Report vulnerabilities privately via GitHub's advisory flow, not public issues.

---

## 12. Testing strategy

- **Core operations:** deterministic unit tests on real sample PDFs (kept small, non-confidential).
- **Privacy invariants:** byte-level assertions that redacted/scrubbed data is absent from output; a test that fails the build if the core module graph gains a network dependency.
- **Engines:** contract tests against the interface so a swapped engine must satisfy the same behavior.
- **AI features:** tests run against a stub/mock inference backend so they are deterministic and offline in CI.

---

## 13. Instructions specific to Claude

- **Do NOT add Claude as a commit co-author** in this repository. Commit as the repository owner only. Do not add `Co-Authored-By` trailers.
- **Never push to `main`.** Always branch and open a PR, even for docs.
- Before finishing any change, re-check the invariants in section 2. If a change adds a dependency or a network capability to the core, stop and flag it in the PR rather than proceeding.
- Avoid em dashes in prose and public markdown (maintainer preference); use commas, hyphens, or restructure.
- When a Phase 0 decision (stack, PDF engine) is made, update sections 6 and 9 and add an ADR under `docs/adr/`.
