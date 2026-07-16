#include <QtTest>

#include <algorithm>
#include <string>

#include "core/PdfRenderer.h"

using sheafly::core::PdfRenderer;

namespace {
std::string dataPath(const char* name) {
    return std::string(SHEAFLY_TEST_DATA_DIR) + "/" + name;
}
}  // namespace

class TestPdfRenderer : public QObject {
    Q_OBJECT

private slots:
    void rendersFirstPage();
    void countsPages();
    void rejectsBadPageIndex();
    void reportsErrorForMissingFile();
};

void TestPdfRenderer::rendersFirstPage() {
    std::string error;
    const auto page = PdfRenderer::renderPage(dataPath("sample.pdf"), 0, 1.0, error);

    QVERIFY2(page.has_value(), error.c_str());
    // sample.pdf is US Letter: 612 x 792 points, scale 1.0.
    QCOMPARE(page->width, 612);
    QCOMPARE(page->height, 792);
    QVERIFY(page->stride >= page->width * 4);
    QCOMPARE(page->bgra.size(), static_cast<size_t>(page->stride) * page->height);

    // The page is empty, so every pixel must be opaque white.
    const bool allWhite = std::all_of(page->bgra.begin(), page->bgra.end(),
                                      [](uint8_t b) { return b == 0xFF; });
    QVERIFY(allWhite);
}

void TestPdfRenderer::countsPages() {
    std::string error;
    const auto count = PdfRenderer::pageCount(dataPath("sample.pdf"), error);

    QVERIFY2(count.has_value(), error.c_str());
    QCOMPARE(*count, 2);
}

void TestPdfRenderer::rejectsBadPageIndex() {
    std::string error;
    const auto page = PdfRenderer::renderPage(dataPath("sample.pdf"), 99, 1.0, error);

    QVERIFY(!page.has_value());
    QVERIFY(!error.empty());
}

void TestPdfRenderer::reportsErrorForMissingFile() {
    std::string error;
    const auto page = PdfRenderer::renderPage(dataPath("does-not-exist.pdf"), 0, 1.0, error);

    QVERIFY(!page.has_value());
    QVERIFY(!error.empty());
}

QTEST_MAIN(TestPdfRenderer)
#include "test_pdfrenderer.moc"
