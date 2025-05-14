#ifndef TADCOLA_H
#define TADCOLA_H

#include <iostream>
#include "Common.h"

#define INCREMENTO 4

using namespace std;

class cola {
    Jugador *elementos;
    int inicio;
    int fin;
    int Tama;
    int ne;

public:
    cola();
    ~cola();

    void encolar(Jugador e);
    void desencolar();
    bool esvacia();
    Jugador primero();
    int longitud();
};

#endif // TADCOLA_H
