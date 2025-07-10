#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Personaje();
    virtual ~Personaje() {}

    void setVida(int v);
    int getVida() const;
    void restarVida(int cantidad);
    void curar(int cantidad);

    void setVelocidad(int v);
    int getVelocidad() const;

protected:
    int vida;
    int velocidad;
};

#endif // PERSONAJE_H
