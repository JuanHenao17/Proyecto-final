#include "enemigo.h"
#include "proyectil.h"
#include <QGraphicsScene>
#include <QRandomGenerator>

Enemigo::Enemigo(qreal posGoku) {

    QPixmap img(":/imagenes/imagenes/enemigo.png");

    int posX;
    do{
        posX = QRandomGenerator::global()->bounded(200, 1080);
    } while(std::abs(posX - posGoku) < 200);

    // Definir si está del lado izquierdo o derecho
    if (posX > 640) {
        img = img.transformed(QTransform().scale(-1, 1));  // Girar horizontalmente
        mirandoIzquierda = true;
    } else {
        mirandoIzquierda = false;
    }

    setPixmap(img.scaled(130, 130));
    setPos(posX, 600);

    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Enemigo::disparar);
    timerDisparo->start(1200); // Dispara cada 2 segundos
}

void Enemigo::disparar() {

    qreal xStart = x() + pixmap().width() / 2;
    qreal yStart = y() + pixmap().height() / 2;

    Proyectil* bala = new Proyectil(mirandoIzquierda, xStart, yStart);
    scene()->addItem(bala);

}
