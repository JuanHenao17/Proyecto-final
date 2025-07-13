#include "roca.h"

Roca::Roca() {
    setPixmap(QPixmap(":/sprites/roca.png").scaled(120, 120));
    velocidad = 25;
}

void Roca::mover() {
    setY(y() + velocidad);
}
