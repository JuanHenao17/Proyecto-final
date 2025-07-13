#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QObject>
#include "goku.h"
#include "enemigo.h"

class Juego: public QObject {

    Q_OBJECT

public:

    Juego(QGraphicsView* view, QObject* parent = nullptr);
    void iniciarNivel1();
    void iniciarNivel2();

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
    bool ganoNivel = false;

    void limpiarEscena();
    QList<Enemigo*> enemigos;
    int enemigosEliminados = 0;
    int enemigosParaGanar = 10;
};

#endif // JUEGO_H
