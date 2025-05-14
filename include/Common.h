#ifndef COMMON_H
#define COMMON_H

typedef char cadena[50];

struct Jugador {
    cadena nombreJugador;
    int ID;
    bool activo;
    int latencia;
    long puntuacion;
    cadena pais;
};

#endif // COMMON_H
