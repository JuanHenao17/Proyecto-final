#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setFixedSize(1280, 720);

    // MENÚ PRINCIPAL
    menuWidget = new QWidget(this);
    menuWidget->setStyleSheet(
        "QWidget {"
        "background-image: url(:/sprites/fondo.jpg);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "}"
        );

    btnIniciar = new QPushButton("Iniciar Juego");
    btnSalir = new QPushButton("Salir");

    btnIniciar->setFixedSize(300, 60);
    btnSalir->setFixedSize(300, 60);

    btnIniciar->setStyleSheet("font-size: 20px; padding: 10px;");
    btnSalir->setStyleSheet("font-size: 20px; padding: 10px;");

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
    vLayout->addSpacing(40);
    vLayout->addLayout(hLayout2);
    vLayout->addStretch();

    menuWidget->setLayout(vLayout);
    setCentralWidget(menuWidget);

    connect(btnIniciar, &QPushButton::clicked, this, &MainWindow::iniciarJuego);
    connect(btnSalir, &QPushButton::clicked, this, &MainWindow::salir);
}

void MainWindow::iniciarJuego() {

    view = new QGraphicsView();
    view->setFixedSize(1280, 720);
    setCentralWidget(view);

    juego = new Juego(view, this);
    juego->iniciarNivel1();  // o iniciarNivel2();

}


void MainWindow::salir() {
    close();
}

MainWindow::~MainWindow(){}
