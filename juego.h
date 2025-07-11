#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QObject>
#include "goku.h"

class Juego: public QObject {

    Q_OBJECT

public:

    Juego(QGraphicsView* view, QObject* parent = nullptr);
    void iniciarNivel1();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:

    QGraphicsView* view;
    QGraphicsScene* scene;
    QTimer* timer;

    Goku* goku;
    QGraphicsRectItem* vidaBar;
    QGraphicsTextItem* tiempoText;

    int tiempoRestante;
    int nivelActual;
    bool nivelTerminado;

    void limpiarEscena();

};

#endif // JUEGO_H
