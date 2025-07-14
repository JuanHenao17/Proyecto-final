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
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    if (scene) {
        scene->removeEventFilter(this);
        scene->clear();
        view->setScene(nullptr);
        delete scene;
        scene = nullptr;
    }

    enemigos.clear();
    enemigosEliminados = 0;
}

void Juego::iniciarNivel1() {
    limpiarEscena();



    sonidoFondoNivel1 = new QMediaPlayer(this);
    salidaAudio = new QAudioOutput(this);
    sonidoFondoNivel1->setAudioOutput(salidaAudio);
    sonidoFondoNivel1->setSource(QUrl("qrc:/sonidos/sonidos/fondo_nivel1.wav"));
    sonidoFondoNivel1->setLoops(QMediaPlayer::Infinite);
    salidaAudio->setVolume(0.5);
    sonidoFondoNivel1->play();


    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1280, 720);


    QPixmap fondoPixmap(":/imagenes/imagenes/cueva_fondo.jpg");
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(1280, 720));
    fondo->setZValue(-1);
    scene->addItem(fondo);


    goku = new Goku();
    goku->setPos(640, 550);
    scene->addItem(goku);


    int fontId = QFontDatabase::addApplicationFont(":/fuentes/fuentes/PressStart.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont fuentePixel(fontFamily, 20);


    notaNivel = new QGraphicsTextItem("Nivel 1:\n¡Evita las rocas durante 90 segundos!");
    notaNivel->setFont(fuentePixel);
    notaNivel->setDefaultTextColor(Qt::white);
    notaNivel->setZValue(99);
    QRectF bounds = notaNivel->boundingRect();
    notaNivel->setPos((1280 - bounds.width()) / 2, (720 - bounds.height()) / 2);
    scene->addItem(notaNivel);


    int barraAncho = 200;
    int barraX = (1280 - barraAncho) / 2;

    QGraphicsRectItem* marcoVida = new QGraphicsRectItem(0, 0, barraAncho, 20);
    marcoVida->setBrush(Qt::black);
    marcoVida->setPen(QPen(Qt::lightGray, 2));
    marcoVida->setPos(barraX, 13);
    marcoVida->setZValue(1);
    scene->addItem(marcoVida);

    vidaBar = new QGraphicsRectItem(0, 0, goku->getVida() * 2, 20);
    vidaBar->setBrush(Qt::red);
    vidaBar->setPos(barraX, 13);
    vidaBar->setZValue(2);
    scene->addItem(vidaBar);

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
    tiempoText->setPos(1000, 12);
    tiempoText->setZValue(4);
    scene->addItem(tiempoText);


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


        vidaBar->setRect(0, 0, goku->getVida() * 2, 20);


        static int contadorFrame = 0;
        contadorFrame++;
        if (contadorFrame >= 33) {
            tiempoRestante--;
            tiempoText->setPlainText("TIEMPO: " + QString::number(tiempoRestante));
            sombraTiempo->setPlainText("TIEMPO: " + QString::number(tiempoRestante));
            contadorFrame = 0;

            if (tiempoRestante <= 0) {
                timer->stop();
                nivelTerminado = true;
                ganoNivel = true;


                if (sonidoFondoNivel1) sonidoFondoNivel1->stop();


                sonidoVictoria = new QMediaPlayer(this);
                salidaVictoria = new QAudioOutput(this);
                sonidoVictoria->setAudioOutput(salidaVictoria);
                sonidoVictoria->setSource(QUrl("qrc:/sonidos/sonidos/victoria.mp3"));
                salidaVictoria->setVolume(0.8);
                sonidoVictoria->play();


                scene->removeItem(tiempoText);
                delete tiempoText;


                QGraphicsPixmapItem* ganasteImg = new QGraphicsPixmapItem(
                    QPixmap(":/imagenes/imagenes/ganaste.png").scaled(400, 400)
                    );
                ganasteImg->setPos(440, 150);
                scene->addItem(ganasteImg);


                QTimer::singleShot(3000, this, [=]() {
                    iniciarNivel2();
                });
            }

        }


        if (goku->getVida() <= 0) {
            timer->stop();
            nivelTerminado = true;
            ganoNivel = false;


            if (sonidoFondoNivel1) sonidoFondoNivel1->stop();

            QGraphicsPixmapItem* perdisteImg = new QGraphicsPixmapItem(
                QPixmap(":/imagenes/imagenes/perdiste.png").scaled(400, 400)
                );
            perdisteImg->setPos(440, 150);
            scene->addItem(perdisteImg);
        }
    });

    timer->start(30);


    QTimer::singleShot(4000, this, [=]() {
        if (notaNivel) {
            scene->removeItem(notaNivel);
            delete notaNivel;
            notaNivel = nullptr;
        }
    });
}


void Juego::iniciarNivel2() {
    limpiarEscena();

    if (sonidoVictoria) {
        sonidoVictoria->stop();
        delete sonidoVictoria;
        sonidoVictoria = nullptr;
    }

    if (salidaVictoria) {
        delete salidaVictoria;
        salidaVictoria = nullptr;
    }

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1280, 720);


    QPixmap fondoPixmap(":/imagenes/imagenes/fondo_submarino.png");
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(1280, 720));
    scene->addItem(fondo);
    fondo->setZValue(-1);


    goku = new Goku();
    goku->setPos(100, 550);
    scene->addItem(goku);


    int fontId = QFontDatabase::addApplicationFont(":/fuentes/fuentes/PressStart.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont fuentePixel(fontFamily, 16);


    notaNivel = new QGraphicsTextItem("Nivel 2:\n¡Elimina a 10 enemigos!");
    notaNivel->setFont(fuentePixel);
    notaNivel->setDefaultTextColor(Qt::white);
    notaNivel->setZValue(99);


    QRectF bounds = notaNivel->boundingRect();
    notaNivel->setPos(
        (1280 - bounds.width()) / 2,
        (720 - bounds.height()) / 2
        );


    scene->addItem(notaNivel);

    int barraAncho = 200;
    int barraX = (1280 - barraAncho) / 2;


    QGraphicsRectItem* marcoVida = new QGraphicsRectItem(0, 0, barraAncho, 20);
    marcoVida->setBrush(Qt::black);
    marcoVida->setPen(QPen(Qt::lightGray, 2));
    marcoVida->setPos(barraX, 13);
    marcoVida->setZValue(1);
    scene->addItem(marcoVida);


    vidaBar = new QGraphicsRectItem(0, 0, goku->getVida() * 2, 20);
    vidaBar->setBrush(Qt::red);
    vidaBar->setPos(barraX, 13);
    vidaBar->setZValue(2);
    scene->addItem(vidaBar);


    sombraNombre = new QGraphicsTextItem("GOKU   LVL   2");
    sombraNombre->setFont(fuentePixel);
    sombraNombre->setDefaultTextColor(Qt::black);
    sombraNombre->setPos(10, 11);
    sombraNombre->setZValue(3);
    scene->addItem(sombraNombre);

    textoNombre = new QGraphicsTextItem("GOKU   LVL   2");
    textoNombre->setFont(fuentePixel);
    textoNombre->setDefaultTextColor(Qt::white);
    textoNombre->setPos(6, 14);
    textoNombre->setZValue(4);
    scene->addItem(textoNombre);


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


    sombraEnemigos = new QGraphicsTextItem("ENEMIGOS 0/10");
    sombraEnemigos->setFont(fuentePixel);
    sombraEnemigos->setDefaultTextColor(Qt::black);
    sombraEnemigos->setPos(980, 11);
    sombraEnemigos->setZValue(3);
    scene->addItem(sombraEnemigos);

    textoEnemigos = new QGraphicsTextItem("ENEMIGOS 0/10");
    textoEnemigos->setFont(fuentePixel);
    textoEnemigos->setDefaultTextColor(Qt::white);
    textoEnemigos->setPos(980, 14);
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


        vidaBar->setRect(0, 0, goku->getVida() * 2, 20);


        int vidaActual = goku->getVida();
        textoVida->setPlainText(QString::number(vidaActual) + "/100");
        sombraVida->setPlainText(QString::number(vidaActual) + "/100");

        static int spawnCounter = 0;
        spawnCounter++;
        if (spawnCounter >= 100) {
            Enemigo* enemigo = new Enemigo(goku->x());
            enemigos.append(enemigo);
            scene->addItem(enemigo);
            spawnCounter = 0;
        }

        for (int i = enemigos.size() - 1; i >= 0; --i) {
            Enemigo* enemigo = enemigos[i];

            if (goku->collidesWithItem(enemigo)) {
                if (goku->y() + goku->boundingRect().height() <= enemigo->y() + 30) {
                    enemigosEliminados++;

                    QString enemigosTexto = "ENEMIGOS " + QString::number(enemigosEliminados) + "/10";
                    textoEnemigos->setPlainText(enemigosTexto);
                    sombraEnemigos->setPlainText(enemigosTexto);

                    scene->removeItem(enemigo);
                    enemigos.removeAt(i);
                    delete enemigo;
                } else {
                    goku->restarVida(20);
                }
            }
        }

        if (enemigosEliminados >= enemigosParaGanar) {
            timer->stop();
            nivelTerminado = true;


            if (sonidoFondoNivel1) sonidoFondoNivel1->stop();


            sonidoVictoria = new QMediaPlayer(this);
            salidaVictoria = new QAudioOutput(this);
            sonidoVictoria->setAudioOutput(salidaVictoria);
            sonidoVictoria->setSource(QUrl("qrc:/sonidos/sonidos/victoria.mp3"));
            salidaVictoria->setVolume(0.8);
            sonidoVictoria->play();


            for (Enemigo* enemigo : enemigos) {
                scene->removeItem(enemigo);
                delete enemigo;
            }
            enemigos.clear();


            QPixmap img(":/imagenes/imagenes/victoria.png");
            QGraphicsPixmapItem* victoriaSprite = new QGraphicsPixmapItem(
                img.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
                );
            victoriaSprite->setZValue(100);
            victoriaSprite->setPos(0, 0);
            scene->addItem(victoriaSprite);

            victoriaMostrada = true;
        }


        if (goku->getVida() <= 0) {
            timer->stop();
            nivelTerminado = true;

            QGraphicsPixmapItem* perdisteImg = new QGraphicsPixmapItem(
                QPixmap(":/imagenes/imagenes/perdiste2.png").scaled(400, 400)
                );
            perdisteImg->setPos(440, 150);
            scene->addItem(perdisteImg);
        }
    });

    timer->start(30);


    QTimer::singleShot(4000, this, [=]() {
        if (notaNivel) {
            scene->removeItem(notaNivel);
            delete notaNivel;
            notaNivel = nullptr;
        }
    });
}



bool Juego::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);


        if (victoriaMostrada &&
            (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)) {
            qDebug() << "🎉 Victoria confirmada. Cerrando juego...";
            QCoreApplication::quit();  // 🔚 Cierra el juego
            return true;
        }


        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            if (notaNivel) {
                scene->removeItem(notaNivel);
                delete notaNivel;
                notaNivel = nullptr;

                timer->start(30);
                return true;
            }


            if (nivelTerminado) {
                if (nivelActual == 1) {
                    if (ganoNivel) {
                        nivelActual = 2;
                        iniciarNivel2();
                    } else {
                        iniciarNivel1();
                    }
                } else if (nivelActual == 2) {
                    iniciarNivel2();
                }
                return true;
            }
        }
    }


    return QObject::eventFilter(obj, event);
}
