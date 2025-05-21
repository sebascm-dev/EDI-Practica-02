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
    GestorServidores();
    ~GestorServidores();
    int getNumServidores();
    bool desplegarServidor(cadena dS, cadena nJ, int i, int mxL, int mxC, int p, cadena lG);
    bool desconetarServidor(cadena dS);
    bool conectarServidor(cadena dS);
    bool realizarMantenimiento(cadena dS);
    bool eliminarServidor(cadena dS);
    bool alojarJugador(Jugador j, cadena nomJuego, cadena host, bool &enEspera);
    bool expulsarJugador(cadena nJ, cadena host);
    int getPosicionServidor(cadena dS);
    void mostrarInformacionServidores(int pos);
    bool jugadorConectado(cadena nJ, cadena dS);
    bool jugadorEnEspera(cadena nJ, cadena dS);
    bool jugadorConectado(cadena nJ);
    bool jugadorEnEspera(cadena nJ);
};

#endif // GESTORSERVIDORES_H
