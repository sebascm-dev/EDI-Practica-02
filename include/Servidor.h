#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <cstring>

#include "Common.h"
#include "TADLista.h"
#include "TADCola.h"

class Servidor
{
    cadena direccionServidor;
    cadena nombreJuego;
    int id;
    Servidor *siguienteServidor;
    int maxJugadoresConectados;
    int maxJugadoresEnEspera;
    lista jugadoresConectados;
    cola jugadoresEnEspera;
    cadena estado;
    int puerto;
    cadena localizacionGeografica;

public:
    ~Servidor();
    Servidor(cadena dS, cadena nJ, int i, int mxL, int mxC, int p, cadena lG);
    int getId();
    void getDireccionServidor(cadena dS);
    void setSiguienteServidor(Servidor *pS);
    Servidor* getSiguienteServidor();
    bool conectarJugador(Jugador j);
    bool ponerJugadorEnEspera(Jugador j);
    void mostrarJugadoresConectados();
    void mostrarJugadoresEnEspera();
    bool estaActivo();
    bool activar();
    bool desactivar();
    bool ponerEnMantenimiento();
    void mostrarInformacion();
    bool expulsarJugador(cadena nombre);
    void getNombreJuego(cadena nJ);
    int getPuerto();
    void getLocalizacionGeografica(cadena lG);
    int getMaxJugadoresConectados();
    int getMaxJugadoresEnEspera();
    int getNumJugadoresConectados();
    int getNumJugadoresEnEspera();
    void exportarJugadoresConectados(Jugador *conectados);
    void exportarJugadoresEnEspera(Jugador *enEspera);
};

#endif // SERVIDOR_H
