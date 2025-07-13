#include "enemigo.h"
#include "proyectil.h"
#include <QGraphicsScene>
#include <QRandomGenerator>

Enemigo::Enemigo() {

    QPixmap img(":/sprites/enemigo.png");

    // Definir si está del lado izquierdo o derecho
    int posX = QRandomGenerator::global()->bounded(200, 1080);

    if (posX > 640) {
        img = img.transformed(QTransform().scale(-1, 1));  // Girar horizontalmente
        mirandoIzquierda = true;  // Nuevo atributo (bool) que puedes crear
    } else {
        mirandoIzquierda = false;
    }

    setPixmap(img.scaled(130, 130));
    setPos(posX, 600);

    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Enemigo::disparar);
    timerDisparo->start(1000); // Dispara cada 2 segundos
}

void Enemigo::disparar() {

    qreal xStart = x() + pixmap().width() / 2;
    qreal yStart = y() + pixmap().height() / 2;

    Proyectil* bala = new Proyectil(mirandoIzquierda, xStart, yStart);
    scene()->addItem(bala);

}

void Enemigo::morir() {
    emit eliminado();  // Goku lo eliminó
    scene()->removeItem(this);
    delete this;
}
