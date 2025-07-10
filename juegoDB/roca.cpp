#include "roca.h"

Roca::Roca() {
    setPixmap(QPixmap(":/sprites/roca.png").scaled(110, 110));
}

void Roca::mover() {
    setY(y() + 25);  // Velocidad de caída
}
