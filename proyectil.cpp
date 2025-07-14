#include "proyectil.h"
#include "goku.h"
#include "qgraphicsscene.h"
#include "qtimer.h"

Proyectil::Proyectil(bool haciaIzquierda, qreal x_origen, qreal y_origen): direccionIzquierda(haciaIzquierda),
    velocidadX(8),
    amplitud(30),
    frecuencia(0.03),
    x_inicial(x_origen),
    y_inicial(y_origen)  {

    hojaSprites.load(":/imagenes/imagenes/disparos.png");
    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto).scaled(30,30);

    setPos(x_inicial, y_inicial);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Proyectil::mover);
    timerMovimiento->start(30);

}

void Proyectil::mover() {

    qreal nuevoX = x() + velocidadX;
    qreal nuevoXNeg = x() - velocidadX;
    // Movimiento vertical senoidal
    qreal nuevoY = y_inicial + amplitud * std::sin(frecuencia * (nuevoX - x_inicial));

    if (direccionIzquierda) {
        setPos(nuevoXNeg, nuevoY);
        confSprite();
    } else {
        setPos(nuevoX, nuevoY);
        confSprite();
    }
    // Colisión con Goku

    QList<QGraphicsItem*> colisiones = collidingItems();
    for (QGraphicsItem* item : colisiones) {
        Goku* g = dynamic_cast<Goku*>(item);
        if (g) {
            g->restarVida(20);
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }

    // Si sale de la pantalla
    if (nuevoX > 1280 || nuevoX < 0) {
        scene()->removeItem(this);
        delete this;
    }
}

void Proyectil::confSprite(){

    spriteX = spriteAncho*cont;
    sprite = hojaSprites.copy(spriteX, spriteY, spriteAncho, spriteAlto);
    setPixmap(sprite.scaled(30,30));
    cont++;

    if(cont>=4){cont=0;}

}
