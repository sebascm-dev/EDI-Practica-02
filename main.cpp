#include <iostream>
#include "Common.h"
#include "Servidor.h"

using namespace std;

int main()
{
    Servidor server1("192.168.18.1", "Minecraft", 1, 5, 4, 25565, "Espana");

    cadena direccionServidor, nombreJuego, localizacionServidor;
    int idServidor, puertoServidor, maxJugadoresConectados, maxJugadoresEspera;

    server1.getDireccionServidor(direccionServidor);
    server1.getLocalizacionGeografica(localizacionServidor);
    server1.getNombreJuego(nombreJuego);
    idServidor = server1.getId();
    puertoServidor = server1.getPuerto();
    maxJugadoresConectados = server1.getMaxJugadoresConectados();
    maxJugadoresEspera = server1.getMaxJugadoresEnEspera();

    cout << "=== Datos del Servidor " << direccionServidor << " | " << localizacionServidor << " ===" << endl;
    cout << "ID: " << idServidor << endl;
    cout << "JUEGO: " << nombreJuego << endl;
    cout << "PUERTO: " << puertoServidor << endl;
    cout << "MAX JUGADORES CONECTADO: " << maxJugadoresConectados << endl;
    cout << "MAX JUGADORES EN ESPERA: " << maxJugadoresEspera << endl;
    cout << "================================================" << endl;

    Jugador j1, j2, j3, j4, j5, j6, j7, j8, j9, j10;

    strcpy(j1.nombreJugador, "Sebas");
    strcpy(j2.nombreJugador, "Antonio");
    strcpy(j3.nombreJugador, "Miguel");
    strcpy(j4.nombreJugador, "Santi");
    strcpy(j5.nombreJugador, "Rafa");
    strcpy(j6.nombreJugador, "Pepe");
    strcpy(j7.nombreJugador, "Amir");
    strcpy(j8.nombreJugador, "Pedro");
    strcpy(j9.nombreJugador, "Manuel");
    strcpy(j10.nombreJugador, "Juan");

    j1.activo = false;
    j2.activo = false;
    j3.activo = false;
    j4.activo = false;
    j5.activo = false;
    j6.activo = false;
    j7.activo = false;
    j8.activo = false;
    j9.activo = false;
    j10.activo = false;

    j1.ID = 1;
    j2.ID = 2;
    j3.ID = 3;
    j4.ID = 4;
    j5.ID = 5;
    j6.ID = 6;
    j7.ID = 7;
    j8.ID = 8;
    j9.ID = 9;
    j10.ID = 10;

    j1.latencia = 12;
    j2.latencia = 32;
    j3.latencia = 34;
    j4.latencia = 23;
    j5.latencia = 21;
    j6.latencia = 18;
    j7.latencia = 19;
    j8.latencia = 14;
    j9.latencia = 16;
    j10.latencia = 27;

    j1.puntuacion = 12;
    j2.puntuacion = 32;
    j3.puntuacion = 34;
    j4.puntuacion = 23;
    j5.puntuacion = 21;
    j6.puntuacion = 18;
    j7.puntuacion = 19;
    j8.puntuacion = 14;
    j9.puntuacion = 16;
    j10.puntuacion = 27;

    strcpy(j1.pais, "Espana");
    strcpy(j2.pais, "Espana");
    strcpy(j3.pais, "Espana");
    strcpy(j4.pais, "Espana");
    strcpy(j5.pais, "Espana");
    strcpy(j6.pais, "Espana");
    strcpy(j7.pais, "Espana");
    strcpy(j8.pais, "Espana");
    strcpy(j9.pais, "Espana");
    strcpy(j10.pais, "Espana");

    server1.conectarJugador(j1);
    server1.conectarJugador(j2);
    server1.conectarJugador(j3);
    server1.conectarJugador(j4);
    server1.conectarJugador(j5);
    server1.conectarJugador(j6);
    server1.conectarJugador(j7);
    server1.conectarJugador(j8);
    server1.conectarJugador(j9);
    server1.conectarJugador(j10);

    server1.mostrarJugadoresConectados();
    server1.mostrarJugadoresEnEspera();

    cout << endl;
    cout << "JUGADORES CONECTADOS: " << server1.getNumJugadoresConectados() << endl;
    cout << "JUGADORES EN COLA: " << server1.getNumJugadoresEnEspera() << endl;

    return 0;
}
