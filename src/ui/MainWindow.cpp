#include "ui/MainWindow.h"

#include "core/PdfDocument.h"

#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>

#include <string>

namespace sheafly::ui {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Sheafly");
    resize(720, 480);

    info_ = new QLabel("Open a PDF to see its details.\nNothing leaves your machine.", this);
    info_->setAlignment(Qt::AlignCenter);
    info_->setWordWrap(true);
    setCentralWidget(info_);

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
        return;
    }

    info_->setText(QString("%1\n\nPages: %2\nPDF version: %3\nEncrypted: %4")
                       .arg(path)
                       .arg(info->pageCount)
                       .arg(QString::fromStdString(info->version))
                       .arg(info->encrypted ? "yes" : "no"));
}

}  // namespace sheafly::ui
