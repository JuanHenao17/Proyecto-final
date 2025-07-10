#include "goku.h"
#include <QKeyEvent>

Goku::Goku() {

    hojaSprites1.load(":/sprites/goku_corriendo.png");
    hojaSprites2.load(":/sprites/goku_corriendo_izq.png");
    sprite = hojaSprites1.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite.scaled(200,200));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    velocidadY = 0;
    enElAire = false;
    setVida(100);

}

void Goku::aplicarFisica() {
    // Movimiento horizontal
    if (moviendoIzq && x() > 0) {
        setX(x() - 15);
        confSprite(false);
    } else if (moviendoDer && x() + pixmap().width() < 1280) {
        setX(x() + 15);
        confSprite(true);
    }

    // Movimiento vertical (gravedad)
    if (enElAire) {
        velocidadY += 1;  // gravedad
        setY(y() + velocidadY);

        if (y() >= 550) {
            setY(550);
            velocidadY = 0;
            enElAire = false;
        }
    }
}


void Goku::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_A) {
        moviendoIzq = true;
    } else if (event->key() == Qt::Key_D) {
        moviendoDer = true;
    } else if (event->key() == Qt::Key_W || event->key() == Qt::Key_Space) {
        if (!enElAire) {
            velocidadY = -20;
            enElAire = true;
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

    if(band){

        spriteY = 0;
        spriteX = spriteAncho*cont;
        sprite = hojaSprites1.copy(spriteX, spriteY, spriteAncho, spriteAlto);
        setPixmap(sprite.scaled(200,200));
        cont++;

        if(cont==8){cont=0;}

    }else{

        spriteY = 0;
        spriteX = spriteAncho*cont;
        sprite = hojaSprites2.copy(spriteX, spriteY, spriteAncho, spriteAlto);
        setPixmap(sprite.scaled(200,200));
        cont++;

        if(cont==8){cont=0;}
    }
}
