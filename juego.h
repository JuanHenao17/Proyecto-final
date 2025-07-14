#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QObject>
#include "goku.h"
#include "enemigo.h"


class Juego : public QObject {
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
    bool ganoNivel = false;

    void limpiarEscena();
    QList<Enemigo*> enemigos;
    int enemigosEliminados = 0;
    int enemigosParaGanar = 10;

    QGraphicsTextItem* textoEnemigos;
    QGraphicsTextItem* sombraEnemigos;


};

#endif // JUEGO_H
