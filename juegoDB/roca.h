#ifndef ROCA_H
#define ROCA_H

#include "obstaculo.h"

class Roca : public Obstaculo {
    Q_OBJECT

public:
    Roca();
    void mover() override;
};

#endif // ROCA_H
