#include <QtTest>

#include <string>

#include "core/PdfDocument.h"

using sheafly::core::PdfDocument;

namespace {
std::string dataPath(const char* name) {
    return std::string(SHEAFLY_TEST_DATA_DIR) + "/" + name;
}
}  // namespace

class TestPdfDocument : public QObject {
    Q_OBJECT

private slots:
    void inspectsValidPdf();
    void reportsErrorForMissingFile();
};

void TestPdfDocument::inspectsValidPdf() {
    std::string error;
    const auto info = PdfDocument::inspect(dataPath("sample.pdf"), error);

    QVERIFY2(info.has_value(), error.c_str());
    QCOMPARE(info->pageCount, 2);
    QCOMPARE(QString::fromStdString(info->version), QStringLiteral("1.7"));
    QVERIFY(!info->encrypted);
}

void TestPdfDocument::reportsErrorForMissingFile() {
    std::string error;
    const auto info = PdfDocument::inspect(dataPath("does-not-exist.pdf"), error);

    QVERIFY(!info.has_value());
    QVERIFY(!error.empty());
}

QTEST_MAIN(TestPdfDocument)
#include "test_pdfdocument.moc"
