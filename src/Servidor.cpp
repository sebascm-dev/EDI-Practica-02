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

    for(int i = 1; i <= totalJugadoresEnEspera; i++){
        Jugador j = jugadoresEnEspera.primero();
        jugadoresEnEspera.desencolar();
        jugadoresEnEspera.encolar(j);

        cout << "\t" << i << ". " << j.nombreJugador << " - Puntuacion: " << j.puntuacion;

        if(!j.activo) {
            cout << "\t (DESACTIVADO)";
        }

        cout << endl;
    }
}



// Estado del servidor
bool Servidor::estaActivo() {
    if(strcmp(estado, "ACTIVO") == 0) {
        cout << "[/] SERVIDOR: " << estado << endl;
        return true;
    } else {
        cout << "[/] SERVIDOR: " << estado << endl;
        return false;
    }
}

bool Servidor::activar() {
    if(!estaActivo()) {
        cout << "[~] El servidor se esta ACTIVANDO..." << endl;
        strcpy(estado, "ACTIVO");
        cout << "[OK] El servidor esta: " << estado << endl;
        return true;
    } else {
        cout << "[!] El servidor ya esta ACTIVO." << endl;
        return false;
    }
}

bool Servidor::desactivar() {
    if(estaActivo()) {
        cout << "[~] El servidor se esta DESACTIVANDO..." << endl;
        strcpy(estado, "INACTIVO");
        cout << "[OK] El servidor esta: " << estado << endl;
        return true;
    } else {
        cout << "[!] El servidor ya esta INACTIVO." << endl;
        return false;
    }
}

bool Servidor::ponerEnMantenimiento() {
    if(strcmp(estado, "INACTIVO") == 0) {
        cout << "[~] El servidor se esta poniendo en MANTENIMIENTO..." << endl;
        strcpy(estado, "MANTENIMIENTO");
        cout << "[OK] El servidor esta: " << estado << endl;
        return true;
    } else {
        return false;
    }
}



// Información general
void Servidor::mostrarInformacion() {
    // Obtencion de la latencia media de los jugadores conectados
    float latenciaMedia = 0.0;
    float sumaLatencia = 0.0;

    for(int i = 0; i <= jugadoresConectados.longitud(); i++) {
        sumaLatencia += jugadoresConectados.observar(i).latencia;
    }

    latenciaMedia = sumaLatencia / jugadoresConectados.longitud();
    cout << endl;
    cout << "X===================================================" << endl;
    cout << "| INFORMACION DEL SERVIDOR: " << nombreJuego << " - " << direccionServidor << endl;
    cout << "X===================================================" << endl;
    cout << "| Estado: " << estado << endl;
    cout << "| Jugadores: " << jugadoresConectados.longitud() << "/" << maxJugadoresConectados << endl;
    cout << "| Jugadores Esperando: " << jugadoresEnEspera.longitud() << "/" << maxJugadoresEnEspera << endl;
    cout << "| Puerto: " << puerto << endl;
    cout << "| Latencia Media: " << latenciaMedia << endl;
    cout << "| Localizacion Geografica: " << localizacionGeografica << endl;
    cout << "X===================================================" << endl;
    cout << endl;
}

bool Servidor::expulsarJugador(cadena nombre) {
    cout << endl << endl;


    int posConectado = 1;
    int sizeLista = jugadoresConectados.longitud();
    int sizeCola = jugadoresEnEspera.longitud();
    bool haExpulsado = false;


    cout << "[i] Buscando jugador en la lista de conectados..." << endl;
    while(posConectado <= sizeLista) {
        Jugador actual = jugadoresConectados.observar(posConectado);

        if(strcmp(actual.nombreJugador, nombre) == 0) {
            jugadoresConectados.eliminar(posConectado);
            cout << "[-] Jugador " << nombre << " expulsado." << endl;
            haExpulsado = true;

            cout << "[i] Comprobando si hay jugadores para unirse..." << endl;
            if(!jugadoresEnEspera.esvacia()) {
                Jugador siguiente = jugadoresEnEspera.primero();
                jugadoresEnEspera.desencolar();
                conectarJugador(siguiente);
                cout << "[+] El jugador " << siguiente.nombreJugador << " se ha unido correctamente." << endl;
            } else {
                cout << "[!] No hay jugadores esperando para conectarse..." << endl;
            }
            return true;

        } else {
            ++posConectado;
        }
    }


    cout << "[i] Buscando jugador en la cola de espera..." << endl;
    for(int i = 0; i < sizeCola; i++) {
        Jugador actual = jugadoresEnEspera.primero();
        jugadoresEnEspera.desencolar();

        if(strcmp(actual.nombreJugador, nombre) == 0) {
            cout << "[-] Jugador " << nombre << " expulsado." << endl;
            haExpulsado = true;

        } else {
            jugadoresEnEspera.encolar(actual);
        }
    }


    if(haExpulsado) {
        return true;

    } else {
        cout << "[!] Jugador " << nombre << " no ha sido encontrado." << endl;
        return false;
    }
}



// Más accesores
void Servidor::getNombreJuego(cadena nJ) { strcpy(nJ, nombreJuego); }

int Servidor::getPuerto() { return puerto; }

void Servidor::getLocalizacionGeografica(cadena lG) { strcpy(lG, localizacionGeografica); }

int Servidor::getMaxJugadoresConectados() { return maxJugadoresConectados; }

int Servidor::getMaxJugadoresEnEspera() { return maxJugadoresEnEspera; }

int Servidor::getNumJugadoresConectados() { return jugadoresConectados.longitud(); }

int Servidor::getNumJugadoresEnEspera() { return jugadoresEnEspera.longitud(); }



// Metodos para exportar
void Servidor::exportarJugadoresConectados(Jugador *conectados) {
    // Obtenemos el numero de jugadores que hay realmente conectado
    int nConectados = jugadoresConectados.longitud();

    // Recorremos la lista interna y volcamos en el array
    for(int i = 0; i < nConectados; i++) {
        conectados[i] = jugadoresConectados.observar(i+1);
    }
}

void Servidor::exportarJugadoresEnEspera(Jugador *enEspera) {
    // Obtenemos el numero de jugadores que estan en la cola de espera
    int nEsperando = jugadoresEnEspera.longitud();

    // Recorremos la cola de espera y volcamos los jugadores en el array
    for(int i = 0; i < nEsperando; i++) {
        Jugador actual = jugadoresEnEspera.primero();
        enEspera[i] = actual;
        jugadoresEnEspera.desencolar();
        jugadoresEnEspera.encolar(actual);
    }
}
