#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QTimer>
#include <QObject>

class Enemigo : public Personaje {
    Q_OBJECT

public:
    Enemigo();
    void disparar();
    void mover(); // Movimiento del enemigo si deseas que patrulle
    void morir();

signals:
    void eliminado();  // Se emite cuando Goku elimina al enemigo

private:
    QTimer* timerDisparo;
    bool mirandoIzquierda;
};

#endif // ENEMIGO_H
