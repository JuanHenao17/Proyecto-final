#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1280, 720);

    // === MENÚ PRINCIPAL ===
    menuWidget = new QWidget(this);
    menuWidget->setStyleSheet(
        "QWidget {"
        "background-image: url(:/imagenes/imagenes/fondo.jpg);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "background-size: cover;"
        "}"
        );

    // === BOTONES ===
    btnIniciar = new QPushButton("INICIAR JUEGO");
    btnSalir = new QPushButton("SALIR");

    btnIniciar->setFixedSize(300, 60);
    btnSalir->setFixedSize(300, 60);

    QString estiloBoton = R"(
        QPushButton {
            background-color: rgba(0, 0, 0, 240);
            color: white;
            font-family: 'Press Start', monospace;
            font-size: 14px;
            border: 2px solid black;
            border-radius: 12px;
            padding: 10px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 50);
            color: yellow;
            border: 2px solid yellow;
        }
    )";

    btnIniciar->setStyleSheet(estiloBoton);
    btnSalir->setStyleSheet(estiloBoton);

    // === LAYOUT ===
    QVBoxLayout* vLayout = new QVBoxLayout();

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addStretch();
    hLayout1->addWidget(btnIniciar);
    hLayout1->addStretch();

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addStretch();
    hLayout2->addWidget(btnSalir);
    hLayout2->addStretch();

    vLayout->addStretch();
    vLayout->addLayout(hLayout1);
    vLayout->addSpacing(30);
    vLayout->addLayout(hLayout2);
    vLayout->addStretch();

    menuWidget->setLayout(vLayout);
    setCentralWidget(menuWidget);

    // === CONEXIONES ===
    connect(btnIniciar, &QPushButton::clicked, this, &MainWindow::iniciarJuego);
    connect(btnSalir, &QPushButton::clicked, this, &MainWindow::salir);
}

// === INICIO DEL JUEGO ===
void MainWindow::iniciarJuego() {
    view = new QGraphicsView();
    view->setFixedSize(1280, 720);
    setCentralWidget(view);

    juego = new Juego(view, this);
    juego->iniciarNivel1();
}

// === SALIR ===
void MainWindow::salir() {
    close();
}

MainWindow::~MainWindow() {}
