#include "juego.h"
#include "roca.h"
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>

Juego::Juego(QGraphicsView* view, QObject* parent)
    : QObject(parent), view(view), scene(nullptr), timer(nullptr), goku(nullptr),
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
                    QPixmap(":/imagenes/imagenes/ganaste.png").scaled(400, 400)
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
                QPixmap(":/imagenes/imagenes/perdiste.png").scaled(400, 400)
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
    QPixmap fondoPixmap(":/imagenes/imagenes/fondo_submarino.png");
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(1280, 720));
    scene->addItem(fondo);
    fondo->setZValue(-1);

    // Goku reaparece
    goku = new Goku();
    goku->setPos(100, 550);
    scene->addItem(goku);

    // Fuente
    int fontId = QFontDatabase::addApplicationFont(":/fuentes/fuentes/PressStart.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont fuentePixel(fontFamily, 16);

    int barraAncho = 200;
    int barraX = (1280 - barraAncho) / 2;

    // Vida
    QGraphicsRectItem* marcoVida = new QGraphicsRectItem(0, 0, barraAncho, 20);
    marcoVida->setBrush(Qt::black);
    marcoVida->setPen(QPen(Qt::lightGray, 2));
    marcoVida->setPos(barraX, 30);
    marcoVida->setZValue(1);
    scene->addItem(marcoVida);

    vidaBar = new QGraphicsRectItem(0, 0, goku->getVida() * 2, 20);
    vidaBar->setBrush(Qt::red);
    vidaBar->setPos(barraX, 30);
    vidaBar->setZValue(2);
    scene->addItem(vidaBar);

    sombraNombre = new QGraphicsTextItem("GOKU   LVL   2");
    sombraNombre->setFont(fuentePixel);
    sombraNombre->setDefaultTextColor(Qt::black);
    sombraNombre->setPos(barraX + 1, 6);
    sombraNombre->setZValue(3);
    scene->addItem(sombraNombre);

    textoNombre = new QGraphicsTextItem("GOKU   LVL   2");
    textoNombre->setFont(fuentePixel);
    textoNombre->setDefaultTextColor(Qt::white);
    textoNombre->setPos(barraX, 5);
    textoNombre->setZValue(4);
    scene->addItem(textoNombre);

    sombraVida = new QGraphicsTextItem("100/100");
    sombraVida->setFont(fuentePixel);
    sombraVida->setDefaultTextColor(Qt::black);
    sombraVida->setPos(barraX + barraAncho + 11, 31);
    sombraVida->setZValue(3);
    scene->addItem(sombraVida);

    textoVida = new QGraphicsTextItem("100/100");
    textoVida->setFont(fuentePixel);
    textoVida->setDefaultTextColor(Qt::white);
    textoVida->setPos(barraX + barraAncho + 10, 30);
    textoVida->setZValue(4);
    scene->addItem(textoVida);

    // ENEMIGOS RESTANTES agregado
    sombraEnemigos = new QGraphicsTextItem("ENEMIGOS 0/20");
    sombraEnemigos->setFont(fuentePixel);
    sombraEnemigos->setDefaultTextColor(Qt::black);
    sombraEnemigos->setPos(31, 71);
    sombraEnemigos->setZValue(3);
    scene->addItem(sombraEnemigos);

    textoEnemigos = new QGraphicsTextItem("ENEMIGOS 0/20");
    textoEnemigos->setFont(fuentePixel);
    textoEnemigos->setDefaultTextColor(Qt::white);
    textoEnemigos->setPos(30, 70);
    textoEnemigos->setZValue(4);
    scene->addItem(textoEnemigos);

    view->setScene(scene);
    view->setFixedSize(1280, 720);
    view->setFocus();

    scene->installEventFilter(this);
    nivelTerminado = false;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&]() {
        goku->aplicarFisica();

        // Actualiza barra de vida
        vidaBar->setRect(0, 0, goku->getVida() * 2, 20);

        static int spawnCounter = 0;
        spawnCounter++;
        if (spawnCounter >= 100) {  // Cada 3 segundos aprox (100 * 30 ms)
            Enemigo* enemigo = new Enemigo(goku->x());
            enemigos.append(enemigo);
            scene->addItem(enemigo);
            spawnCounter = 0;
        }

        for (int i = enemigos.size() - 1; i >= 0; --i) {
            Enemigo* enemigo = enemigos[i];

            if (goku->collidesWithItem(enemigo)) {
                if (goku->y() + goku->boundingRect().height() <= enemigo->y() + 30) {
                    // Goku pisa al enemigo
                    enemigosEliminados++;

                    // ✅ Actualiza contador en pantalla
                    QString enemigosTexto = "ENEMIGOS " + QString::number(enemigosEliminados) + "/20";
                    textoEnemigos->setPlainText(enemigosTexto);
                    sombraEnemigos->setPlainText(enemigosTexto);

                    scene->removeItem(enemigo);
                    enemigos.removeAt(i);
                    delete enemigo;
                } else {
                    // Golpe lateral
                    goku->restarVida(20);

                    // ✅ Actualiza texto de vida
                    int vidaActual = goku->getVida();
                    textoVida->setPlainText(QString::number(vidaActual) + "/100");
                    sombraVida->setPlainText(QString::number(vidaActual) + "/100");
                }
            }
        }

        if (enemigosEliminados >= enemigosParaGanar) {
            timer->stop();
            nivelTerminado = true;

            QGraphicsPixmapItem* ganasteImg = new QGraphicsPixmapItem(
                QPixmap(":/imagenes/imagenes/ganaste.png").scaled(400, 400)
                );
            ganasteImg->setPos(440, 150);
            scene->addItem(ganasteImg);
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
