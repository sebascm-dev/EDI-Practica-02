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
    bool expulsarJugador(cadena nJ, cadena host);
    int getPosicionServidor(cadena dS);
    void mostrarInformacionServidores(int pos);
    bool jugadorConectado(cadena nJ, cadena dS);
    bool jugadorEnEspera(cadena nJ, cadena dS);
    bool jugadorConectado(cadena nJ);
    bool jugadorEnEspera(cadena nJ);
};

#endif // GESTORSERVIDORES_H
