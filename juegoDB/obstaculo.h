#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Obstaculo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Obstaculo();
    virtual void mover() = 0; // método puro, cada subclase define su movimiento

protected:
    int velocidad;
};

#endif // OBSTACULO_H
