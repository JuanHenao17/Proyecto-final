#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // Iniciar ventana principal
    MainWindow w;
    w.show();

    return a.exec();
}
