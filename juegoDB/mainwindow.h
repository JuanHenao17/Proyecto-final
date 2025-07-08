#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //void iniciarJuego();
    void salir();

private:

    QWidget* menuWidget;
    QPushButton* btnIniciar;
    QPushButton* btnSalir;

};
#endif // MAINWINDOW_H
