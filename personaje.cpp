#include "personaje.h"

Personaje::Personaje() {
    vida = 100;
    velocidad = 5;
}

void Personaje::setVida(int v) {
    vida = v;
}

int Personaje::getVida() const {
    return vida;
}

void Personaje::restarVida(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;
}

void Personaje::curar(int cantidad) {
    vida += cantidad;
    if (vida > 100) vida = 100;
}

void Personaje::setVelocidad(int v) {
    velocidad = v;
}

int Personaje::getVelocidad() const {
    return velocidad;
}
