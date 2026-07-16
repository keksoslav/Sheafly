#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sheafly::core {

// One rendered page as a raw BGRA pixel buffer (4 bytes per pixel,
// little-endian, rows padded to `stride` bytes).
struct PageImage {
    int width = 0;
    int height = 0;
    int stride = 0;
    std::vector<uint8_t> bgra;
};

// Renders PDF pages via PDFium.
//
// Core layer: no Qt, no UI, no network. The UI converts PageImage into
// whatever its framework wants (QImage etc.).
class PdfRenderer {
public:
    // Renders page `pageIndex` (0-based) scaled by `scale` (1.0 = 72 dpi).
    // On failure returns nullopt and writes a message into `error`.
    static std::optional<PageImage> renderPage(const std::string& path,
                                               int pageIndex,
                                               double scale,
                                               std::string& error);

    // Number of pages, or nullopt on failure.
    static std::optional<int> pageCount(const std::string& path, std::string& error);
};

}  // namespace sheafly::core
