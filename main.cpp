#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Verificar si el recurso de fuente existe
    bool fuenteExiste = QFile(":/fuentes/fuentes/PressStart.ttf").exists();
    qDebug() << "¿La fuente PressStart2P.ttf existe en el recurso?"
             << (fuenteExiste ? "✅ Sí" : "❌ No");

    // Cargar la fuente desde los recursos
    int fontId = QFontDatabase::addApplicationFont(":/fuentes/fuentes/PressStart.ttf");
    if (fontId == -1) {
        qDebug() << "❌ Error: No se pudo cargar la fuente PressStart.ttf";
    } else {
        QStringList familias = QFontDatabase::applicationFontFamilies(fontId);
        if (familias.isEmpty()) {
            qDebug() << "⚠️ Fuente cargada, pero no se encontraron familias.";
        } else {
            qDebug() << "✅ Fuente cargada correctamente: " << familias.at(0);
        }
    }

    // Iniciar ventana principal
    MainWindow w;
    w.show();

    return a.exec();
}
