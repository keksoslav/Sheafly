# Sheafly Roadmap

Sheafly aims to be a feature-complete PDF24 alternative — and then go past it on three pillars: **privacy/offline**, **true editing**, and **local AI**.

## Legend

- `[ ]` planned · `[~]` in progress · `[x]` shipped

---

## Phase 0 — Foundations

- [ ] Pick PDF engine (PDFium / MuPDF / Apryse) and app stack (Tauri / Qt)
- [ ] Core PDF document model + renderer
- [ ] CI, packaging, auto-update (offline-friendly)

## Phase 1 — PDF24 parity (table stakes)

### Organize
- [ ] Merge PDFs
- [ ] Split PDF
- [ ] Extract / delete / reorder / rotate pages

### Optimize
- [ ] Compress
- [ ] Flatten

### Convert
- [ ] To PDF from Word / Excel / PPT / images / HTML / text
- [ ] From PDF to Word / images / text / HTML

### Edit & annotate
- [ ] Annotations (highlight, notes, shapes)
- [ ] Form filling

### Security
- [ ] Password protect / unlock
- [ ] Redact
- [ ] Sign

### Extras
- [ ] OCR
- [ ] Watermark
- [ ] Page numbers
- [ ] Extract images
- [ ] Compare / diff

---

## Phase 2 — Privacy / offline (pillar)

- [ ] Guaranteed zero-network core + visible offline indicator / kill switch
- [ ] Metadata scrubber (author, GPS, producer, timestamps, history)
- [ ] Verifiable redaction (removes underlying data + proof report)
- [ ] Hidden-data audit (layers, embedded files, JS, off-page content)
- [ ] Local encrypted vault

## Phase 3 — True editing (pillar)

- [ ] Real text editing with reflow + font matching
- [ ] Find-and-replace across the whole document
- [ ] Vector / image edit in place (crop, replace)
- [ ] Visual + text diff between two versions

## Phase 4 — Local AI (pillar, opt-in)

- [ ] Pluggable inference engine (bundled small model or user's local endpoint)
- [ ] Chat-with-PDF with page-cited answers
- [ ] Summarize document / selection
- [ ] Table / invoice extraction → clean CSV / JSON
- [ ] Auto-generate bookmarks / TOC from headings
- [ ] Layout-preserving translation
- [ ] Smart rename from content
- [ ] Alt-text generation (accessibility)

## Phase 5 — Power users (later)

- [ ] Batch processing with saved recipes
- [ ] Watch-folder automation
- [ ] CLI + scriptable core
- [ ] Data merge (CSV + template → many PDFs)
- [ ] Form builder + auto-detect fillable fields
- [ ] PDF/A + PDF/UA (accessibility) compliance
