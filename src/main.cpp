#include "ui/MainWindow.h"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Sheafly");
    QApplication::setOrganizationName("Sheafly");

    sheafly::ui::MainWindow window;
    window.show();

    return app.exec();
}
