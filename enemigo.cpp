#include "enemigo.h"
#include "proyectil.h"
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTransform>
#include <QUrl>

Enemigo::Enemigo(qreal posGoku) {
    QPixmap img(":/imagenes/imagenes/enemigo.png");

    int posX;
    do {
        posX = QRandomGenerator::global()->bounded(200, 1080);
    } while (std::abs(posX - posGoku) < 200);

    if (posX > 640) {
        img = img.transformed(QTransform().scale(-1, 1));
        mirandoIzquierda = true;
    } else {
        mirandoIzquierda = false;
    }

    setPixmap(img.scaled(130, 130));
    setPos(posX, 600);

    // === SONIDO DEL DISPARO ===
    efectoDisparo = new QSoundEffect(this);
    efectoDisparo->setSource(QUrl("qrc:/sonidos/sonidos/disparo.wav"));
    efectoDisparo->setVolume(0.7); // volumen entre 0.0 y 1.0

    // === TEMPORIZADOR DE DISPARO ===
    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Enemigo::disparar);
    timerDisparo->start(1200);
}

void Enemigo::disparar() {
    if (efectoDisparo->isLoaded()) {
        efectoDisparo->play();
    }

    qreal xStart = x() + pixmap().width() / 2;
    qreal yStart = y() + pixmap().height() / 2;

    Proyectil* bala = new Proyectil(mirandoIzquierda, xStart, yStart);
    scene()->addItem(bala);
}
