#pragma once

#include <QMainWindow>

class QLabel;

namespace sheafly::ui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void openPdf();

private:
    QLabel* info_ = nullptr;
};

}  // namespace sheafly::ui
