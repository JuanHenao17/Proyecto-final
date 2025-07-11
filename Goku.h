#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include <QKeyEvent>

class Goku : public Personaje {
public:

    Goku();
    void aplicarFisica();
    void confSprite(bool band);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:

    int spriteX = 0;
    int spriteY = 0;
    int spriteAncho = 68;
    int spriteAlto = 86;
    QPixmap hojaSprites1;

    QPixmap hojaSprites2;
    QPixmap sprite;
    QPixmap spriteSaltoDerecha;
    QPixmap spriteSaltoIzquierda;

    int cont=0;

    int velocidadY;
    bool enElAire;
    bool moviendoIzq = false;
    bool moviendoDer = false;
    bool mirandoDerecha = true;
};

#endif // GOKU_H
