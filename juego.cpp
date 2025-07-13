#include "juego.h"
#include "roca.h"
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>

Juego::Juego(QGraphicsView* view, QObject* parent)
    : QObject(parent), view(view), scene(nullptr), timer(nullptr),
    vidaBar(nullptr), tiempoText(nullptr), tiempoRestante(0), nivelActual(1) {}

void Juego::limpiarEscena() {
    if (scene) {
        QList<QGraphicsItem*> items = scene->items();
        for (QGraphicsItem* item : items) {
            scene->removeItem(item);
            delete item;
        }
        delete scene;
        scene = nullptr;
    }
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
}

void Juego::iniciarNivel1() {
    limpiarEscena();
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1280, 720);

    // Fondo
    QPixmap fondoPixmap(":/imagenes/imagenes/cueva_fondo.jpg");
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(1280, 720));
    fondo->setZValue(-1);
    scene->addItem(fondo);

    // Goku
    goku = new Goku();
    goku->setPos(640, 550);
    scene->addItem(goku);

    // Fuente más grande
    int fontId = QFontDatabase::addApplicationFont(":/fuentes/fuentes/PressStart.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont fuentePixel(fontFamily, 20);  // Aumentado de 12 a 16

    // Posición centrada de la barra de vida
    int barraAncho = 200;
    int barraX = (1280 - barraAncho) / 2; // 540

    // Marco de vida
    QGraphicsRectItem* marcoVida = new QGraphicsRectItem(0, 0, barraAncho, 20);
    marcoVida->setBrush(Qt::black);
    marcoVida->setPen(QPen(Qt::lightGray, 2));
    marcoVida->setPos(barraX, 13);
    marcoVida->setZValue(1);
    scene->addItem(marcoVida);

    // Barra de vida roja
    vidaBar = new QGraphicsRectItem(0, 0, goku->getVida() * 2, 20);
    vidaBar->setBrush(Qt::red);
    vidaBar->setPos(barraX, 14);
    vidaBar->setZValue(2);
    scene->addItem(vidaBar);

    // Texto centrado arriba de la barra
    sombraNombre = new QGraphicsTextItem("GOKU   LVL   1");
    sombraNombre->setFont(fuentePixel);
    sombraNombre->setDefaultTextColor(Qt::black);
    sombraNombre->setPos(10, 6);
    sombraNombre->setZValue(3);
    scene->addItem(sombraNombre);

    textoNombre = new QGraphicsTextItem("GOKU   LVL   1");
    textoNombre->setFont(fuentePixel);
    textoNombre->setDefaultTextColor(Qt::white);
    textoNombre->setPos(11, 10);
    textoNombre->setZValue(4);
    scene->addItem(textoNombre);

    // Vida numérica a la derecha de la barra
    sombraVida = new QGraphicsTextItem("100/100");
    sombraVida->setFont(fuentePixel);
    sombraVida->setDefaultTextColor(Qt::black);
    sombraVida->setPos(barraX + barraAncho + 11, 6);
    sombraVida->setZValue(3);
    scene->addItem(sombraVida);

    textoVida = new QGraphicsTextItem("100/100");
    textoVida->setFont(fuentePixel);
    textoVida->setDefaultTextColor(Qt::white);
    textoVida->setPos(barraX + barraAncho + 10, 11);
    textoVida->setZValue(4);
    scene->addItem(textoVida);

    // Tiempo (mantiene su posición en esquina superior derecha)
    tiempoRestante = 90;

    sombraTiempo = new QGraphicsTextItem("TIEMPO: 90");
    sombraTiempo->setFont(fuentePixel);
    sombraTiempo->setDefaultTextColor(Qt::black);
    sombraTiempo->setPos(1000, 11);
    sombraTiempo->setZValue(3);
    scene->addItem(sombraTiempo);

    tiempoText = new QGraphicsTextItem("TIEMPO: 90");
    tiempoText->setFont(fuentePixel);
    tiempoText->setDefaultTextColor(Qt::white);
    tiempoText->setPos(1000, 14);
    tiempoText->setZValue(4);
    scene->addItem(tiempoText);

    // Escena y timer
    view->setScene(scene);
    view->setFixedSize(1280, 720);
    view->setFocus();
    scene->installEventFilter(this);
    nivelTerminado = false;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&]() {
        goku->aplicarFisica();

        if (QRandomGenerator::global()->bounded(30) == 0) {
            Roca* roca = new Roca();
            roca->setPos(QRandomGenerator::global()->bounded(1280), 0);
            scene->addItem(roca);
        }

        QList<QGraphicsItem*> items = scene->items();
        for (QGraphicsItem* item : items) {
            Roca* roca = dynamic_cast<Roca*>(item);
            if (roca) {
                roca->mover();
                if (roca->y() > 720) {
                    scene->removeItem(roca);
                    delete roca;
                } else if (roca->collidesWithItem(goku)) {
                    goku->restarVida(20);
                    scene->removeItem(roca);
                    delete roca;

                    int vidaActual = goku->getVida();
                    textoVida->setPlainText(QString::number(vidaActual) + "/100");
                    sombraVida->setPlainText(QString::number(vidaActual) + "/100");
                }
            }
        }

        // Actualizar barra de vida
        vidaBar->setRect(0, 0, goku->getVida() * 2, 20);

        // Actualizar tiempo
        static int contadorFrame = 0;
        contadorFrame++;
        if (contadorFrame >= 33) {
            tiempoRestante--;
            QString tiempoStr = "TIEMPO " + QString::number(tiempoRestante);
            tiempoText->setPlainText(tiempoStr);
            sombraTiempo->setPlainText(tiempoStr);
            contadorFrame = 0;

            if (tiempoRestante <= 0) {
                timer->stop();
                nivelTerminado = true;

                scene->removeItem(tiempoText);
                scene->removeItem(sombraTiempo);
                delete tiempoText;
                delete sombraTiempo;

                QGraphicsPixmapItem* ganasteImg = new QGraphicsPixmapItem(
                    QPixmap(":/imagenes/imagenes/ganaste.png").scaled(400, 400));
                ganasteImg->setPos(440, 150);
                scene->addItem(ganasteImg);
            }
        }

        if (goku->getVida() <= 0) {
            timer->stop();
            nivelTerminado = true;

            QGraphicsPixmapItem* perdisteImg = new QGraphicsPixmapItem(
                QPixmap(":/imagenes/imagenes/perdiste.png").scaled(400, 400));
            perdisteImg->setPos(440, 150);
            scene->addItem(perdisteImg);
        }
    });

    timer->start(30);
}

bool Juego::eventFilter(QObject* obj, QEvent* event) {
    if (nivelTerminado && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            iniciarNivel1();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}
