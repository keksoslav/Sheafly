#pragma once

#include <optional>
#include <string>

namespace sheafly::core {

// Basic structural facts about a PDF, read without rendering it.
struct PdfInfo {
    int pageCount = 0;
    std::string version;   // e.g. "1.7"
    bool encrypted = false;
};

// Reads a PDF from disk and reports basic info.
//
// This lives in the core layer: no Qt, no UI, no network. Everything here
// must work with the machine offline. Rendering and editing come later and
// stay behind their own interfaces.
class PdfDocument {
public:
    // Returns the info on success. On failure returns nullopt and writes a
    // human-readable message into `error`.
    static std::optional<PdfInfo> inspect(const std::string& path, std::string& error);
};

}  // namespace sheafly::core
