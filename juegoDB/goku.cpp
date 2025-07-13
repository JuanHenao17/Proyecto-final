#include "goku.h"
#include "qtimer.h"
#include <QKeyEvent>

Goku::Goku() {

    hojaSprites1.load(":/sprites/goku_corriendo_izq.png");
    sprite = hojaSprites1.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    spriteSaltoDerecha = QPixmap(":/sprites/goku_saltando.png").copy(0, 0, 100, 119);
    spriteSaltoIzquierda = spriteSaltoDerecha.transformed(QTransform().scale(-1, 1));

    setPixmap(sprite.scaled(200,200));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    velocidadY = 0;
    enElAire = false;
    setVida(100);

    timerInmunidad = new QTimer(this);
    timerInmunidad->setSingleShot(true);
    connect(timerInmunidad, &QTimer::timeout, this, [=]() {
        inmune = false;
        setVisible(true);
        timerParpadeo->stop();
        setOpacity(1.0);
    });

    timerParpadeo = new QTimer(this);
    connect(timerParpadeo, &QTimer::timeout, this, [=]() {
        setOpacity(isOpaque() ? 0.3 : 1.0);
    });
}

void Goku::aplicarFisica() {
    // Movimiento horizontal
    if (!enElAire) {
        if (moviendoIzq && x() > 0) {
            setX(x() - 15);
            mirandoDerecha = false;
            confSprite(false);
        } else if (moviendoDer && x() + pixmap().width() < 1280) {
            setX(x() + 15);
            mirandoDerecha = true;
            confSprite(true);
        }
    } else {
        // Si se está moviendo en el aire, actualizar posición, pero mantener sprite de salto
        if (moviendoIzq && x() > 0) {
            setX(x() - 10);
            mirandoDerecha = false;
        } else if (moviendoDer && x() + pixmap().width() < 1280) {
            setX(x() + 10);
            mirandoDerecha = true;
        }
    }

    // Movimiento vertical (gravedad)
    if (enElAire) {
        velocidadY += 1;  // gravedad
        setY(y() + velocidadY);

        if (y() >= 550) {
            setY(550);
            velocidadY = 0;
            enElAire = false;
            confSprite(mirandoDerecha);
        } else{
            if (mirandoDerecha){
                setPixmap(spriteSaltoDerecha.scaled(200, 200));}
            else {
                 setPixmap(spriteSaltoIzquierda.scaled(200, 200));}
        }
    }
}


void Goku::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_A) {
        moviendoIzq = true;
        mirandoDerecha = false;
    } else if (event->key() == Qt::Key_D) {
        moviendoDer = true;
        mirandoDerecha = true;
    } else if (event->key() == Qt::Key_W || event->key() == Qt::Key_Space) {
        if (!enElAire) {
            velocidadY = -20;
            enElAire = true;

            if (mirandoDerecha)
                setPixmap(spriteSaltoDerecha.scaled(200, 200));
            else
                setPixmap(spriteSaltoIzquierda.scaled(200, 200));
        }
    }
}

void Goku::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_A) {
        moviendoIzq = false;
    } else if (event->key() == Qt::Key_D) {
        moviendoDer = false;
    }
}

void Goku::confSprite(bool band){

    spriteY = 0;
    spriteX = spriteAncho*cont;

    if(band){

        QImage img = hojaSprites1.copy(spriteX, spriteY, spriteAncho, spriteAlto).toImage();
        img = img.mirrored(true, false);
        sprite = QPixmap::fromImage(img);

    }else{

        sprite = hojaSprites1.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    }

    setPixmap(sprite.scaled(200,200));
    cont++;

    if(cont>=8){cont=0;}

}

bool Goku::isOpaque() const { return opacity() >= 1.0; }
