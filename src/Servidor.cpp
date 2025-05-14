#include "Servidor.h"
#include "Common.h"

using namespace std;

// Constructores y destructores
Servidor::~Servidor() {}

Servidor::Servidor(cadena dS, cadena nJ, int i, int mxL, int mxC, int p, cadena lG) {
    strcpy(direccionServidor, dS);
    strcpy(nombreJuego, nJ);
    id = i;
    maxJugadoresConectados = mxL;
    maxJugadoresEnEspera = mxC;
    puerto = p;
    strcpy(localizacionGeografica, lG);
    strcpy(estado, "INACTIVO");
}



// Accesores y modificadores
int Servidor::getId() { return id; }

void Servidor::getDireccionServidor(cadena dS) { strcpy(dS, direccionServidor); }

void Servidor::setSiguienteServidor(Servidor *pS) { siguienteServidor = pS; }

Servidor* Servidor::getSiguienteServidor() { return siguienteServidor; }



// Gestión de jugadores
bool Servidor::conectarJugador(Jugador j) {
    cout << "[+] Comprobando si el jugador se puede conectar..." << endl;
    if(jugadoresConectados.longitud() >= maxJugadoresConectados) {
        cout << "[!] El servidor esta completo, poniendo el jugador en cola..." << endl;
        bool enEspera = ponerJugadorEnEspera(j);
        if(enEspera){
            cout << "[+] El jugador se ha colocado en la cola del servidor..." << endl;
        } else {
            cout << "[!] La cola del servidor esta llena, intentelo en otro momento..." << endl;
        }
        return false; // NO CABEN MAS
    }

    cout << "[+] Activando la ID del jugador..." << endl;
    j.activo = true;

    cout << "[i] Buscando la posicion para insertar el jugador mediante su puntuacion..." << endl;
    int pos = 1;

    while(pos <= jugadoresConectados.longitud() && j.puntuacion >= jugadoresConectados.observar(pos).puntuacion) {
        ++pos;
    }

    cout << "[+] Posicion encontrada: " << pos << endl;
    jugadoresConectados.insertar(pos, j);

    cout << "[+] Jugador " << j.nombreJugador << " introducido correctamente." << endl;
    return true;
}

bool Servidor::ponerJugadorEnEspera(Jugador j) {
    cout << "[+] Comprobando si el jugador puede entrar en cola..." << endl;
    if(jugadoresEnEspera.longitud() >= maxJugadoresEnEspera) {
        return false;
    }

    j.activo = false;

    cout << "[i] Poniendo el jugador en lista de espera..." << endl;
    jugadoresEnEspera.encolar(j);

    return true;
}

void Servidor::mostrarJugadoresConectados() {
    cout << endl << endl;
    int totalJugadoresConectados = jugadoresConectados.longitud();
    cout << "=== " << totalJugadoresConectados << " Jugadores Conectados en el servidor " << nombreJuego << " ===" << endl;

    if(totalJugadoresConectados == 0) {
        cout << "\t[~] No hay jugadores conectados al servidor..." << endl;
        return;
    }

    for(int i = 1; i <= totalJugadoresConectados; i++) {
        Jugador j = jugadoresConectados.observar(i);

        cout << "\t" << i << ". " << j.nombreJugador << " - Puntuacion: " << j.puntuacion;

        if(j.activo) {
            cout << "\t (ACTIVO)" << endl;
        }
    }
}

void Servidor::mostrarJugadoresEnEspera() {
    cout << endl << endl;
    int totalJugadoresEnEspera = jugadoresEnEspera.longitud();
    cout << "=== " << totalJugadoresEnEspera << " Jugadores esperando para conectarse en el servidor " << nombreJuego << " ===" << endl;

    if(totalJugadoresEnEspera == 0) {
        cout << "\t[~] No hay jugadores esperando para conectarse..." << endl;
        return;
    }

    cola copiaJugadoresEnEspera = jugadoresEnEspera;

    for(int i = 1; i <= totalJugadoresEnEspera; i++){
        Jugador j = copiaJugadoresEnEspera.primero();
        copiaJugadoresEnEspera.desencolar();

        cout << "\t" << i << ". " << j.nombreJugador << " - Puntuacion: " << j.puntuacion;

        if(!j.activo) {
            cout << "\t (DESACTIVADO)" << endl;
        }
    }
}



// Estado del servidor
bool Servidor::estaActivo() {}

bool Servidor::activar() {}

bool Servidor::desactivar() {}

bool Servidor::ponerEnMantenimiento() {}



// Información general
void Servidor::mostrarInformacion() {}

bool Servidor::expulsarJugador(cadena nombre) {}



// Más accesores
void Servidor::getNombreJuego(cadena nJ) { strcpy(nJ, nombreJuego); }

int Servidor::getPuerto() { return puerto; }

void Servidor::getLocalizacionGeografica(cadena lG) { strcpy(lG, localizacionGeografica); }

int Servidor::getMaxJugadoresConectados() { return maxJugadoresConectados; }

int Servidor::getMaxJugadoresEnEspera() { return maxJugadoresEnEspera; }

int Servidor::getNumJugadoresConectados() { return jugadoresConectados.longitud(); }

int Servidor::getNumJugadoresEnEspera() { return jugadoresEnEspera.longitud(); }

void Servidor::exportarJugadoresConectados(Jugador *conectados) {}

void Servidor::exportarJugadoresEnEspera(Jugador *enEspera) {}
