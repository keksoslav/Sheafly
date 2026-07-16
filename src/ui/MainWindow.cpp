#include "ui/MainWindow.h"

#include "core/PdfDocument.h"
#include "core/PdfRenderer.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>
#include <QLabel>
#include <QMenuBar>
#include <QPixmap>
#include <QScrollArea>
#include <QStatusBar>
#include <QVBoxLayout>

#include <cstring>
#include <string>

namespace sheafly::ui {

namespace {

// PageImage is BGRA, which matches QImage::Format_ARGB32 on little-endian.
QImage toQImage(const sheafly::core::PageImage& page) {
    QImage image(page.width, page.height, QImage::Format_ARGB32);
    for (int y = 0; y < page.height; ++y) {
        std::memcpy(image.scanLine(y),
                    page.bgra.data() + static_cast<size_t>(y) * page.stride,
                    static_cast<size_t>(page.width) * 4);
    }
    return image;
}

}  // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Sheafly");
    resize(820, 900);

    auto* container = new QWidget(this);
    auto* layout = new QVBoxLayout(container);

    info_ = new QLabel("Open a PDF to view it.\nNothing leaves your machine.", container);
    info_->setAlignment(Qt::AlignCenter);
    info_->setWordWrap(true);
    layout->addWidget(info_);

    page_ = new QLabel(container);
    page_->setAlignment(Qt::AlignCenter);

    scroll_ = new QScrollArea(container);
    scroll_->setWidget(page_);
    scroll_->setWidgetResizable(true);
    scroll_->setVisible(false);
    layout->addWidget(scroll_, 1);

    setCentralWidget(container);

    QMenu* fileMenu = menuBar()->addMenu("&File");

    QAction* openAct = fileMenu->addAction("&Open PDF...");
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::openPdf);

    fileMenu->addSeparator();

    QAction* quitAct = fileMenu->addAction("&Quit");
    connect(quitAct, &QAction::triggered, this, &QWidget::close);

    statusBar()->showMessage("Offline");
}

void MainWindow::openPdf() {
    const QString path =
        QFileDialog::getOpenFileName(this, "Open PDF", QString(), "PDF files (*.pdf)");
    if (path.isEmpty()) {
        return;
    }

    std::string error;
    const auto info = core::PdfDocument::inspect(path.toStdString(), error);
    if (!info) {
        info_->setText(QString("Could not read this PDF:\n%1").arg(QString::fromStdString(error)));
        scroll_->setVisible(false);
        return;
    }

    info_->setText(QString("%1  |  %2 page(s), PDF %3%4")
                       .arg(QFileInfo(path).fileName())
                       .arg(info->pageCount)
                       .arg(QString::fromStdString(info->version))
                       .arg(info->encrypted ? ", encrypted" : ""));

    // Render page 1 crisply for high-DPI displays.
    const qreal dpr = devicePixelRatioF();
    const auto rendered = core::PdfRenderer::renderPage(path.toStdString(), 0, 1.5 * dpr, error);
    if (!rendered) {
        page_->setText(QString("Could not render page 1:\n%1").arg(QString::fromStdString(error)));
        scroll_->setVisible(true);
        return;
    }

    QImage image = toQImage(*rendered);
    image.setDevicePixelRatio(dpr);
    page_->setPixmap(QPixmap::fromImage(std::move(image)));
    page_->adjustSize();
    scroll_->setVisible(true);
    statusBar()->showMessage(QString("Offline  |  %1").arg(path));
}

}  // namespace sheafly::ui
