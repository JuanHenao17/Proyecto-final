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
    QPixmap fondoPixmap(":/sprites/cueva_fondo.jpg");

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
                ganoNivel = true;

                scene->removeItem(tiempoText);
                delete tiempoText;

                // Muestra imagen de GANASTE
                QGraphicsPixmapItem* ganasteImg = new QGraphicsPixmapItem(
                    QPixmap(":/sprites/ganaste.png").scaled(400, 400)
                    );
                ganasteImg->setPos(440, 150);
                scene->addItem(ganasteImg);
            }
        }

        if (goku->getVida() <= 0) {
            timer->stop();
            nivelTerminado = true;
            ganoNivel = false;

            QGraphicsPixmapItem* perdisteImg = new QGraphicsPixmapItem(
                QPixmap(":/sprites/perdiste.png").scaled(400, 400)
                );
            perdisteImg->setPos(440, 150);
            scene->addItem(perdisteImg);
        }

    });

    timer->start(30);
}

void Juego::iniciarNivel2() {
    limpiarEscena();

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1280, 720);

    // Fondo del nivel 2
    QPixmap fondoPixmap(":/sprites/fondo_submarino.png");
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(1280, 720));
    scene->addItem(fondo);
    fondo->setZValue(-1);

    // Goku reaparece
    goku = new Goku();
    goku->setPos(100, 550);
    scene->addItem(goku);

    // Barra de vida
    vidaBar = new QGraphicsRectItem(0, 0, goku->getVida() * 5, 30);
    vidaBar->setBrush(Qt::red);
    vidaBar->setPos(20, 20);
    scene->addItem(vidaBar);

    view->setScene(scene);
    view->setFixedSize(1280, 720);
    view->setFocus();

    scene->installEventFilter(this);
    nivelTerminado = false;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&]() {
        goku->aplicarFisica();

        // Actualiza vida
        vidaBar->setRect(0, 0, goku->getVida() * 5, 30);

        static int spawnCounter = 0;
        spawnCounter++;
        if (spawnCounter >= 100) {  // Cada 3 segundos aprox (100 * 30 ms)
            Enemigo* enemigo = new Enemigo();
            enemigos.append(enemigo);
            scene->addItem(enemigo);
            spawnCounter = 0;
        }

        for (int i = enemigos.size() - 1; i >= 0; --i) {
            Enemigo* enemigo = enemigos[i];


            // Verifica colisión con Goku (desde arriba)
            if (goku->collidesWithItem(enemigo)) {
                if (goku->y() + goku->boundingRect().height() <= enemigo->y() + 30) {
                    // Goku pisa al enemigo
                    enemigosEliminados++;
                    scene->removeItem(enemigo);
                    enemigos.removeAt(i);
                    delete enemigo;
                } else {
                    // Golpe lateral
                    goku->restarVida(20);
                }
            }
        }

        if (enemigosEliminados >= enemigosParaGanar) {
            timer->stop();
            nivelTerminado = true;

            QGraphicsPixmapItem* ganasteImg = new QGraphicsPixmapItem(
                QPixmap(":/sprites/ganaste.png").scaled(400, 400)
                );
            ganasteImg->setPos(440, 150);
            scene->addItem(ganasteImg);
        }

        if (goku->getVida() <= 0) {
            timer->stop();
            nivelTerminado = true;

            QGraphicsPixmapItem* perdisteImg = new QGraphicsPixmapItem(
                QPixmap(":/sprites/perdiste.png").scaled(400, 400)
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
            if (nivelActual == 1) {
                if (ganoNivel) {
                    nivelActual = 2;
                    iniciarNivel2();
                } else {
                    iniciarNivel1();  // Reintentar nivel 1
                }
            } else if (nivelActual == 2) {
                if (!ganoNivel) {
                    iniciarNivel2();  // Reintentar nivel 2 si pierde
                }
            }

            return true;  // Evento manejado
        }
    }
    return QObject::eventFilter(obj, event);
}
