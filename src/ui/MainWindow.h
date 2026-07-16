#pragma once

#include <QMainWindow>

class QLabel;
class QScrollArea;

namespace sheafly::ui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void openPdf();

private:
    QLabel* info_ = nullptr;
    QLabel* page_ = nullptr;
    QScrollArea* scroll_ = nullptr;
};

}  // namespace sheafly::ui
