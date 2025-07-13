#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QObject>
#include "goku.h"

class Juego : public QObject {
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

    // HUD
    QGraphicsTextItem* tiempoText;
    QGraphicsTextItem* sombraTiempo;

    QGraphicsTextItem* textoVida;
    QGraphicsTextItem* sombraVida;

    QGraphicsTextItem* textoNombre;
    QGraphicsTextItem* sombraNombre;

    int tiempoRestante;
    int nivelActual;
    bool nivelTerminado;

    void limpiarEscena();
};

#endif // JUEGO_H
