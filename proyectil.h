#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "obstaculo.h"

class Proyectil : public Obstaculo {
    Q_OBJECT

public:

    Proyectil(bool haciaIzquierda, qreal x_origen, qreal y_origen);
    void mover() override;
    void confSprite();

private:

    int spriteX = 0;
    int spriteY = 48;
    int spriteAncho = 48;
    int spriteAlto = 48;
    QPixmap hojaSprites;
    QPixmap sprite;
    int cont = 0;
    bool direccionIzquierda;

    QTimer* timerMovimiento;
    int velocidadX;     // velocidad horizontal
    double amplitud;    // amplitud de la oscilación
    double frecuencia;  // frecuencia de la oscilación
    double x_inicial;   // para registrar la posición inicial en x
    double y_inicial;   // posición base en y para aplicar seno

};

#endif // PROYECTIL_H
