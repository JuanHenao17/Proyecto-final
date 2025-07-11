#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "juego.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void iniciarJuego();
    void salir();

private:

    QGraphicsView* view;
    Juego* juego;

    QWidget* menuWidget;
    QPushButton* btnIniciar;
    QPushButton* btnSalir;

};
#endif // MAINWINDOW_H
