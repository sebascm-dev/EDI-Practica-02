#ifndef GESTORSERVIDORES_H
#define GESTORSERVIDORES_H

#include <cstring>

#include "Common.h"
#include "TADLista.h"
#include "TADCola.h"
#include "Servidor.h"

class GestorServidores {
    Servidor *primerServidor;
    int numServidores;

public:
    GestorServidores(); // [OK]
    ~GestorServidores(); // [OK]
    int getNumServidores(); // [OK]
    bool desplegarServidor(cadena dS, cadena nJ, int i, int mxL, int mxC, int p, cadena lG); // [OK]
    bool desconectarServidor(cadena dS); // [OK]
    bool conectarServidor(cadena dS); // [OK]
    bool realizarMantenimiento(cadena dS); // [OK]
    bool eliminarServidor(cadena dS); // [OK]
    bool alojarJugador(Jugador j, cadena nomJuego, cadena host, bool &enEspera); // [OK]
    bool expulsarJugador(cadena nJ, cadena host); // [OK
    int getPosicionServidor(cadena dS); // [OK
    void mostrarInformacionServidores(int pos); // [OK
    bool jugadorConectado(cadena nJ, cadena dS); // [OK
    bool jugadorEnEspera(cadena nJ, cadena dS); // [OK
    bool jugadorConectado(cadena nJ); // [OK
    bool jugadorEnEspera(cadena nJ); // [OK
    Servidor* getServidorMasPuntuacion(cadena nombreJuego);
};

#endif // GESTORSERVIDORES_H
