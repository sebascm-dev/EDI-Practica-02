#ifndef TADLISTA_H
#define TADLISTA_H

#include <iostream>
#include "Common.h"

#define INCREMENTO 4

using namespace std;

class lista {
    Jugador *elementos;
    int n;
    int Tama;

public:
    lista();
    ~lista();
    lista(Jugador e);

    bool esvacia();
    int longitud();
    void anadirIzq(Jugador e);
    void anadirDch(Jugador e);
    void eliminarIzq();
    void eliminarDch();
    Jugador observarIzq();
    Jugador observarDch();
    void concatenar(lista l);
    bool pertenece(Jugador e);

    void insertar(int i, Jugador e);
    void eliminar(int i);
    void modificar(int i, Jugador e);
    Jugador observar(int i);
    int posicion(Jugador e);
};

#endif // TADLISTA_H
