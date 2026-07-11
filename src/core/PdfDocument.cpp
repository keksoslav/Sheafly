#include "core/PdfDocument.h"

#include <qpdf/QPDF.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>

#include <exception>

namespace sheafly::core {

std::optional<PdfInfo> PdfDocument::inspect(const std::string& path, std::string& error) {
    try {
        QPDF pdf;
        pdf.processFile(path.c_str());

        PdfInfo info;
        QPDFPageDocumentHelper pages(pdf);
        info.pageCount = static_cast<int>(pages.getAllPages().size());
        info.version = pdf.getPDFVersion();
        info.encrypted = pdf.isEncrypted();
        return info;
    } catch (const std::exception& e) {
        // Password-protected files land here too, until we add a password prompt.
        error = e.what();
        return std::nullopt;
    }
}

}  // namespace sheafly::core
