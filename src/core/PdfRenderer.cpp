#include "core/PdfRenderer.h"

#include <fpdfview.h>

#include <cmath>
#include <cstring>
#include <fstream>

namespace sheafly::core {

namespace {

// PDFium wants a single global init/destroy. Tie it to process lifetime.
struct PdfiumLibrary {
    PdfiumLibrary() { FPDF_InitLibrary(); }
    ~PdfiumLibrary() { FPDF_DestroyLibrary(); }
};

void ensureLibrary() {
    static PdfiumLibrary library;
}

// Reads the whole file. Loading from memory instead of handing PDFium a
// path avoids platform path-encoding surprises.
std::optional<std::vector<uint8_t>> readFile(const std::string& path, std::string& error) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in) {
        error = "could not open file: " + path;
        return std::nullopt;
    }
    const auto size = in.tellg();
    std::vector<uint8_t> data(static_cast<size_t>(size));
    in.seekg(0);
    in.read(reinterpret_cast<char*>(data.data()), size);
    if (!in) {
        error = "could not read file: " + path;
        return std::nullopt;
    }
    return data;
}

std::string lastPdfiumError() {
    switch (FPDF_GetLastError()) {
        case FPDF_ERR_FILE:     return "file not found or could not be read";
        case FPDF_ERR_FORMAT:   return "file is not a PDF or is corrupted";
        case FPDF_ERR_PASSWORD: return "file is password protected";
        case FPDF_ERR_SECURITY: return "unsupported security scheme";
        case FPDF_ERR_PAGE:     return "page not found or content error";
        default:                return "unknown PDF error";
    }
}

}  // namespace

std::optional<PageImage> PdfRenderer::renderPage(const std::string& path,
                                                 int pageIndex,
                                                 double scale,
                                                 std::string& error) {
    ensureLibrary();

    const auto data = readFile(path, error);
    if (!data) {
        return std::nullopt;
    }

    FPDF_DOCUMENT doc = FPDF_LoadMemDocument(data->data(), static_cast<int>(data->size()), nullptr);
    if (!doc) {
        error = lastPdfiumError();
        return std::nullopt;
    }

    std::optional<PageImage> result;
    if (pageIndex < 0 || pageIndex >= FPDF_GetPageCount(doc)) {
        error = "page index out of range";
    } else if (FPDF_PAGE page = FPDF_LoadPage(doc, pageIndex)) {
        const int width = static_cast<int>(std::ceil(FPDF_GetPageWidthF(page) * scale));
        const int height = static_cast<int>(std::ceil(FPDF_GetPageHeightF(page) * scale));

        FPDF_BITMAP bitmap = FPDFBitmap_CreateEx(width, height, FPDFBitmap_BGRA, nullptr, 0);
        if (bitmap) {
            FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFFF);
            FPDF_RenderPageBitmap(bitmap, page, 0, 0, width, height, 0, FPDF_ANNOT);

            PageImage image;
            image.width = width;
            image.height = height;
            image.stride = FPDFBitmap_GetStride(bitmap);
            const auto* buffer = static_cast<const uint8_t*>(FPDFBitmap_GetBuffer(bitmap));
            image.bgra.assign(buffer, buffer + static_cast<size_t>(image.stride) * height);
            result = std::move(image);

            FPDFBitmap_Destroy(bitmap);
        } else {
            error = "could not allocate page bitmap";
        }
        FPDF_ClosePage(page);
    } else {
        error = lastPdfiumError();
    }

    FPDF_CloseDocument(doc);
    return result;
}

std::optional<int> PdfRenderer::pageCount(const std::string& path, std::string& error) {
    ensureLibrary();

    const auto data = readFile(path, error);
    if (!data) {
        return std::nullopt;
    }

    FPDF_DOCUMENT doc = FPDF_LoadMemDocument(data->data(), static_cast<int>(data->size()), nullptr);
    if (!doc) {
        error = lastPdfiumError();
        return std::nullopt;
    }
    const int count = FPDF_GetPageCount(doc);
    FPDF_CloseDocument(doc);
    return count;
}

}  // namespace sheafly::core
