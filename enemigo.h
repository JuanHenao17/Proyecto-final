#pragma once

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QSoundEffect>

class Enemigo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Enemigo(qreal posGoku);

private slots:
    void disparar();

private:
    QTimer* timerDisparo;
    bool mirandoIzquierda;

    QSoundEffect* efectoDisparo;
};
