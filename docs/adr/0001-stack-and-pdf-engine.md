# 1. App stack and PDF engine

Date: 2026-07-11
Status: accepted

## Context

Sheafly needs a desktop app stack and a PDF engine before any feature work. The choice has to fit the project's constraints:

- Offline and privacy first, so the core must be easy to keep free of network dependencies.
- True in-place editing later, which is the hardest requirement and leans on the engine.
- MIT licensed, so any dependency has to be license-compatible.
- Solo maintainer whose strongest language is C++.
- Cross-platform desktop, developed on Windows.

## Decision

**Stack: Qt 6 with C++.** Single language, native desktop UI, links PDF and local-inference libraries directly, and plays to the maintainer's existing C++ and embedded experience. Developed with the MSYS2 UCRT64 toolchain (GCC, CMake, Ninja), which also drives CI.

**PDF engine: PDFium for rendering and page-object editing, QPDF for structural operations** (merge, split, encrypt, inspect). QPDF is already integrated. PDFium arrives in a later PR via prebuilt binaries, since it is not packaged for MSYS2 and is impractical to build from source.

## Alternatives considered

- **Tauri (Rust + web UI):** best story for a provably offline core and the smallest binaries, but adds a new language and a web UI layer. Set aside to move faster in C++.
- **Electron (TypeScript):** largest UI ecosystem, but heavy and the weakest fit for the performance and privacy goals.
- **MuPDF engine:** strong rendering and editing, but AGPL, which would force Sheafly off MIT. Rejected.
- **Apryse / PDFTron:** true editing out of the box, but paid commercial. Rejected.

## Consequences

- The core layer (`src/core`) stays pure C++ with no Qt or network dependencies. Qt is confined to the UI layer.
- Contributors need the MSYS2 UCRT64 toolchain with `qt6-base`, `qt6-tools`, `qpdf`, `cmake`, and `ninja`.
- True text editing remains the biggest open risk and will be built on PDFium's page-object APIs.
- If cross-platform packaging or the offline-core guarantee later proves painful in this stack, revisit Tauri.
