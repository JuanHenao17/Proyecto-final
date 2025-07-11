#include "juego.h"
#include "roca.h"
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QFont>
#include <QDebug>

Juego::Juego(QGraphicsView* view, QObject* parent)
    : QObject(parent), view(view), scene(nullptr), timer(nullptr)/*, goku(nullptr)*/,
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

void Juego::iniciarNivel1(){

    limpiarEscena();
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1280, 720);

    // Fondo de la cueva
    QPixmap fondoPixmap(":/imagenes/imagenes/cueva_fondo.jpg");

    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(1280, 720));
    scene->addItem(fondo);
    fondo->setZValue(-1);

    // Goku
    goku = new Goku();
    goku->setPos(640, 550);
    scene->addItem(goku);

    // Barra de vida
    vidaBar = new QGraphicsRectItem(0, 0, goku->getVida() * 5, 30);
    vidaBar->setBrush(Qt::red);
    vidaBar->setPos(20, 20);
    scene->addItem(vidaBar);

    // Contador de tiempo
    tiempoRestante = 90;  // 2 minutos
    tiempoText = new QGraphicsTextItem();
    tiempoText->setPlainText("Tiempo: 90");
    tiempoText->setDefaultTextColor(Qt::white);
    tiempoText->setFont(QFont("Arial", 20));
    tiempoText->setPos(20, 60);
    scene->addItem(tiempoText);

    view->setScene(scene);
    view->setFixedSize(1280, 720);
    view->setFocus();

    // Instalar filtro de eventos para detectar ENTER
    scene->installEventFilter(this);

    nivelTerminado = false;  // Reinicia flag

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&]() {
        goku->aplicarFisica();

        // Crear roca aleatoria
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
                }
            }
        }

        vidaBar->setRect(0, 0, goku->getVida() * 5, 30);

        // Contador de tiempo
        static int contadorFrame = 0;
        contadorFrame++;
        if (contadorFrame >= 33) {
            tiempoRestante--;
            tiempoText->setPlainText("Tiempo: " + QString::number(tiempoRestante));
            contadorFrame = 0;

            if (tiempoRestante <= 0) {
                timer->stop();
                nivelTerminado = true;

                // Elimina texto antiguo si quieres
                scene->removeItem(tiempoText);
                delete tiempoText;

                // Muestra imagen de GANASTE
                QGraphicsPixmapItem* ganasteImg = new QGraphicsPixmapItem(
                    QPixmap(":/imagenes/imagenes/ganaste.png").scaled(400, 400)
                    );
                ganasteImg->setPos(440, 150);
                scene->addItem(ganasteImg);
            }
        }

        if (goku->getVida() <= 0) {
            timer->stop();
            nivelTerminado = true;

            QGraphicsPixmapItem* perdisteImg = new QGraphicsPixmapItem(
                QPixmap(":/imagenes/imagenes/perdiste.png").scaled(400, 400)
                );
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
            return true;  // Evento manejado
        }
    }
    return QObject::eventFilter(obj, event);
}
