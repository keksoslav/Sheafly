# Roadmap

The goal is to cover everything a tool like PDF24 does, then push further on three things: offline privacy, real editing, and local AI.

## Phase 0: foundations

- [x] Pick the app stack (Qt 6 / C++) and PDF engine (PDFium + QPDF), see docs/adr/0001
- [x] App skeleton: open a PDF and show its details (pages, version, encrypted)
- [x] CI builds and tests on MSYS2 UCRT64
- [x] PDFium integration: render the first page
- [ ] Packaging and offline-friendly updates

## Phase 1: the basics

Organize
- [ ] Merge
- [ ] Split
- [ ] Extract, delete, reorder, rotate pages

Optimize
- [ ] Compress
- [ ] Flatten

Convert
- [ ] To PDF from Word, Excel, PowerPoint, images, HTML, text
- [ ] From PDF to Word, images, text, HTML

Edit and annotate
- [ ] Annotations (highlight, notes, shapes)
- [ ] Form filling

Security
- [ ] Password protect and unlock
- [ ] Redact
- [ ] Sign

Extras
- [ ] OCR
- [ ] Watermark
- [ ] Page numbers
- [ ] Extract images
- [ ] Compare two files

## Phase 2: privacy and offline

- [ ] Zero-network core with a visible offline indicator and a hard kill switch
- [ ] Metadata scrubber (author, GPS, producer, timestamps, history)
- [ ] Verifiable redaction that removes the underlying data and reports what it removed
- [ ] Hidden-data audit (layers, embedded files, JavaScript, off-page content)
- [ ] Local encrypted vault

## Phase 3: real editing

- [ ] Text editing with reflow and font matching
- [ ] Find and replace across the whole document
- [ ] Vector and image editing in place (crop, replace)
- [ ] Visual and text diff between two versions

## Phase 4: local AI (opt-in)

- [ ] Pluggable backend: bundled small model or the user's own local endpoint
- [ ] Chat with a PDF, answers cited back to pages
- [ ] Summarize a document or a selection
- [ ] Table and invoice extraction to CSV or JSON
- [ ] Auto bookmarks and table of contents from headings
- [ ] Translation that keeps the layout
- [ ] Smart rename from content
- [ ] Alt text for images

## Phase 5: power users

- [ ] Batch processing with saved recipes
- [ ] Watch folders
- [ ] CLI
- [ ] Data merge (CSV plus template into many PDFs)
- [ ] Form builder and auto-detect of fillable fields
- [ ] PDF/A and PDF/UA compliance
