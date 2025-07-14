#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QTimer>
#include <QObject>

class Enemigo : public Personaje {
    Q_OBJECT

public:
    Enemigo(qreal posGoku);
    void disparar();

private:
    QTimer* timerDisparo;
    bool mirandoIzquierda;
};

#endif // ENEMIGO_H
