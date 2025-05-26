#include "GestorServidores.h"
#include "Servidor.h"
#include "Common.h"

// Constructores y destructores
GestorServidores::GestorServidores() {
    primerServidor = nullptr;
    numServidores = 0;
}

GestorServidores::~GestorServidores() {
    Servidor* actual = primerServidor;

    while(actual != nullptr) {
        Servidor* siguiente = actual->getSiguienteServidor();
        delete actual;
        actual = siguiente;
    }

    primerServidor = nullptr;
    numServidores = 0;
}

int GestorServidores::getNumServidores() {
    return numServidores;
}

bool GestorServidores::desplegarServidor(cadena dS, cadena nJ, int i, int mxL, int mxC, int p, cadena lG) {
    // [X] - VALIDACION DE DUPLICADOS DE DIRECCION
    Servidor* actual = primerServidor;

    cout << "[~] Validando si ya existe la direccion: " << dS << endl;
    while(actual != nullptr) {
        cadena direccionActual;
        actual->getDireccionServidor(direccionActual);
        if(strcmp(direccionActual, dS) == 0) {
            cout << "[!] Ya existe un servidor con la direccion: " << direccionActual << endl;
            return false;
        }
        actual = actual->getSiguienteServidor();
    }


    // [X] - VALIDACION DE DUPLICADOS DE ID
    actual = primerServidor;

    cout << "[~] Validando si ya existe la id: " << i << endl;
    while(actual != nullptr) {
        if(actual->getId() == i) {
            cout << "[!] Ya existe un servidor con el ID: " << i << endl;
            return false;
        }
        actual = actual->getSiguienteServidor();
    }


    // [X] - CREACION DEL NUEVO NODO
    cout << "[+] Creando el nuevo Servidor (ID: " << i << "; DIRECCION: " << dS << ")" << endl;
    Servidor* nuevo = new Servidor(dS, nJ, i, mxL, mxC, p, lG);


    // [X] - DETERMINACION DEL PUNTO DE INSERCCION
    Servidor* previo = nullptr;
    actual = primerServidor;
    cadena paisActual;

    cout << "[~] Reconociendo donde se insertara el nuevo servidor..." << endl;
    if(actual != nullptr) {
        actual->getLocalizacionGeografica(paisActual);
    }

    while(actual != nullptr && strcmp(paisActual, lG) < 0) {
        previo = actual;
        actual = actual->getSiguienteServidor();
        if(actual != nullptr) {
            actual->getLocalizacionGeografica(paisActual);
        }
    }


    // [X] - INSERCCION EN LA LISTA ENLAZADA
    if(previo == nullptr) {
        cout << "[+] Insertando el servidor al principio de la lista." << endl;
        nuevo->setSiguienteServidor(primerServidor);
        primerServidor = nuevo;
    } else {
        cout << "[+] Insertando el servidor en la lista." << endl;
        nuevo->setSiguienteServidor(actual);
        previo->setSiguienteServidor(nuevo);
    }

    // [X] - ACTUALIZACION DE CONTADOR
    numServidores++;
    cout << "[+] Actualizando numero de servidores a: " << numServidores << endl;

    return true;
}

bool GestorServidores::desconectarServidor(cadena dS) {
    // [X] - LOCALIZAR EL SERVIDOR A DESACTIVAR
    Servidor* previo = nullptr;
    Servidor* actual = primerServidor;
    bool encontrado = false;

    cout << "[~] Buscando servidor con direccion: " << dS << endl;

    while(actual != nullptr && !encontrado) {
        cadena direccionActual;
        actual->getDireccionServidor(direccionActual);

        if(strcmp(direccionActual, dS) == 0) {
            cout << "[+] Servidor encontrado." << endl;
            encontrado = true;
        } else {
            previo = actual;
            actual = actual->getSiguienteServidor();
        }
    }

    if(!encontrado) {
        cout << "[!] No se encontro ningun servidor con la direccion: " << dS << endl;
        return false;
    }


    // [X] - COMPROBAR EXISTENCIA Y ESTADO
    cout << "[~] Comprobando si el servidor esta activo..." << endl;
    if(!actual->estaActivo()) {
        cout << "[!] El servidor ya se encuentra INACTIVO y no se puede desconectar..." << endl;
        return false;
    }

    cout << "[+] Servidor ACTIVO, procediendo a la desconexion del mismo." << endl;


    // [X] - RECOGER LISTAS DE JUGADORES
    int nConectados = actual->getNumJugadoresConectados();
    int nEnEspera = actual->getNumJugadoresEnEspera();

    cout << "[~] Hay " << nConectados << " jugadores conectados. Guardando en lista..." << endl;
    cout << "[~] Hay " << nEnEspera << " jugadores esperando. Guardando en lista..." << endl;

    Jugador* listaConectados = new Jugador[nConectados];
    Jugador* listaEnEspera = new Jugador[nEnEspera];

    actual->exportarJugadoresConectados(listaConectados);
    actual->exportarJugadoresEnEspera(listaEnEspera);


    // [X] - DESACTIVAR SERVIDOR
    cout << "[-] Desactivando el servidor..." << endl;
    bool exitoDesactivado = actual->desactivar();

    if(!exitoDesactivado) {
        cout << "[!] No se pudo desactivar el servidor (ya estaba INACTIVO?)" << endl;
        delete[] listaConectados;
        delete[] listaEnEspera;
        return false;
    }

    cout << "[+] Servidor desactivado correctamente: " << endl;
    cout << "[/] SERVIDOR: "; actual->estaActivo();

    cadena juegoServidor;
    actual->getNombreJuego(juegoServidor);

    // [X] - REDISTRIBUIR JUGADORES CONECTADOS
    cout << "[~] Organizando jugadores conectados por latencia para redistribuir..." << endl;
    for(int a = 0; a < nConectados - 1; a++) {
        for(int b = a + 1; b < nConectados; b++) {
            if(listaConectados[b].latencia < listaConectados[a].latencia) {
                Jugador temp = listaConectados[a];
                listaConectados[a] = listaConectados[b];
                listaConectados[b] = temp;
            }
        }
    }

    for (int i = 0; i < nConectados; ++i) {
        Jugador j = listaConectados[i];
        cadena   host;
        bool     enEspera;
        bool     ok = alojarJugador(j, juegoServidor, host, enEspera);
        if (ok && !enEspera) {
            cout << "[+] Jugador " << j.nombreJugador
                 << " conectado en " << host << endl;
        } else if (!ok && enEspera) {
            cout << "[+] Jugador " << j.nombreJugador
                 << " en espera en " << host << endl;
        } else {
            cout << "[!] Jugador " << j.nombreJugador
                 << " expulsado del sistema." << endl;
        }
    }
    delete[] listaConectados;


    // [X] - REDISTRIBUIR JUGADORES EN ESPERA
    cout << "[~] Organizando jugadores en espera por latencia para redistribuir..." << endl;
    for(int a = 0; a < nEnEspera - 1; a++) {
        for(int b = a + 1; b < nEnEspera; b++) {
            if(listaEnEspera[b].latencia < listaEnEspera[a].latencia) {
                Jugador temp = listaEnEspera[a];
                listaEnEspera[a] = listaEnEspera[b];
                listaEnEspera[b] = temp;
            }
        }
    }

    for (int i = 0; i < nEnEspera; ++i) {
        Jugador j = listaEnEspera[i];
        cadena host;
        bool enEspera;
        bool ok = alojarJugador(j, juegoServidor, host, enEspera);
        if (ok && !enEspera) {
            cout << "[+] Jugador " << j.nombreJugador
                 << " conectado en " << host << endl;
        } else if (!ok && enEspera) {
            cout << "[+] Jugador " << j.nombreJugador
                 << " en espera en " << host << endl;
        } else {
            cout << "[!] Jugador " << j.nombreJugador
                 << " expulsado del sistema." << endl;
        }
    }
    delete[] listaEnEspera;

    cout << "[OK] Todos los jugadores han sido redistribuidos y el servidor esta inactivo." << endl;

    // [X] - DEVOLVER RESULTADO
    return true;
}

bool GestorServidores::conectarServidor(cadena dS) {
    // [X] - LOCALIZAR EL SERVIDOR A ACTIVAR
    Servidor* previo = nullptr;
    Servidor* actual = primerServidor;
    bool encontrado = false;

    cout << "[~] Buscando servidor con direccion: " << dS << endl;

    while(actual != nullptr && !encontrado) {
        cadena direccionActual;
        actual->getDireccionServidor(direccionActual);

        if(strcmp(direccionActual, dS) == 0) {
            cout << "[+] Servidor encontrado." << endl;
            encontrado = true;
        } else {
            previo = actual;
            actual = actual->getSiguienteServidor();
        }
    }

    if(!encontrado) {
        cout << "[!] No se encontro ningun servidor con la direccion: " << dS << endl;
        return false;
    }


    // [X] - COMPROBAR EXISTENCIA Y ESTADO
    cout << "[~] Comprobando si el servidor esta activo..." << endl;
    if(actual->estaActivo()) {
        cout << "[!] El servidor ya se encuentra ACTIVO y no se puede activar..." << endl;
        return false;
    }

    cout << "[-] Procediendo a la activacion del mismo." << endl;


    // [X] - ACTIVAR SERVIDOR
    bool exitoActivacion = actual->activar();

    if(!exitoActivacion) {
        cout << "[!] No se pudo activar el servidor (ya estaba ACTIVO?)" << endl;
        return false;
    }

    return true;
}

bool GestorServidores::realizarMantenimiento(cadena dS) {
    // [X] - LOCALIZAR EL SERVIDOR A ACTIVAR
    Servidor* previo = nullptr;
    Servidor* actual = primerServidor;
    bool encontrado = false;

    cout << "[~] Buscando servidor con direccion: " << dS << endl;

    while(actual != nullptr && !encontrado) {
        cadena direccionActual;
        actual->getDireccionServidor(direccionActual);

        if(strcmp(direccionActual, dS) == 0) {
            cout << "[+] Servidor encontrado." << endl;
            encontrado = true;
        } else {
            previo = actual;
            actual = actual->getSiguienteServidor();
        }
    }

    if(!encontrado) {
        cout << "[!] No se encontro ningun servidor con la direccion: " << dS << endl;
        return false;
    }


    // [X] - COMPROBAR EXISTENCIA Y ESTADO
    cout << "[~] Estado actual del servidor: ";
    actual->estaActivo();
    if (actual->estaActivo()) {
        cout << "[!] No puede ponerse en mantenimiento porque está ACTIVO." << endl;
        return false;
    }


    // [X] - PONIENDO EN MANTENIMIENTO EL SERVIDOR
    cout << "[~] Poniendo el servidor en MANTENIMIENTO..." << endl;
    bool ok = actual->ponerEnMantenimiento();
    if (!ok) {
        cout << "[!] No se pudo cambiar a MANTENIMIENTO (ya estaba en mantenimiento?)." << endl;
        return false;
    }

    cout << "[OK] Servidor " << dS << " ahora en MANTENIMIENTO." << endl;
    return true;
}

bool GestorServidores::eliminarServidor(cadena dS) {
    // [X] - LOCALIZAR EL SERVIDOR A ACTIVAR
    Servidor* previo = nullptr;
    Servidor* actual = primerServidor;
    bool encontrado = false;

    cout << "[~] Buscando servidor con direccion: " << dS << endl;

    while(actual != nullptr && !encontrado) {
        cadena direccionActual;
        actual->getDireccionServidor(direccionActual);

        if(strcmp(direccionActual, dS) == 0) {
            cout << "[+] Servidor encontrado." << endl;
            encontrado = true;
        } else {
            previo = actual;
            actual = actual->getSiguienteServidor();
        }
    }

    if(!encontrado) {
        cout << "[!] No se encontro ningun servidor con la direccion: " << dS << endl;
        return false;
    }


    // [X] - COMPROBAR EXISTENCIA Y ESTADO
    cout << "[~] Comprobando que el servidor esté INACTIVO o en MANTENIMIENTO..." << endl;
    if (actual->estaActivo()) {
        cout << "[!] No se puede eliminar un servidor ACTIVO." << endl;
        return false;
    }


    // [] - DESENLACE DE LOS NODOS
    if (previo == nullptr) {
        primerServidor = actual->getSiguienteServidor();
    } else {
        previo->setSiguienteServidor(actual->getSiguienteServidor());
    }


    // [] - LIBERAR MEMORIA
    delete[] actual;
    delete[] previo;

    numServidores--;
    cout << "[OK] Servidor eliminado correctamente. Quedan " << numServidores << " servidores." << endl;

    return true;
}

bool GestorServidores::alojarJugador(Jugador j, cadena nomJuego, cadena host, bool &enEspera) {

    Servidor* it = primerServidor;
    Servidor* mejorSrvConn = nullptr;
    int       maxLibres    = -1;
    Servidor* mejorSrvCola = nullptr;
    int       maxCola      = -1;
    cadena    juegoSrv;

    while (it != nullptr) {
        it->getNombreJuego(juegoSrv);
        if (it->estaActivo() && strcmp(juegoSrv, nomJuego) == 0) {
            // huecos libres para conexión
            int libres = it->getMaxJugadoresConectados()
                       - it->getNumJugadoresConectados();
            if (libres > maxLibres) {
                maxLibres    = libres;
                mejorSrvConn = it;
            }
            // huecos libres en cola
            int colaLibres = it->getMaxJugadoresEnEspera()
                           - it->getNumJugadoresEnEspera();
            if (colaLibres > maxCola) {
                maxCola      = colaLibres;
                mejorSrvCola = it;
            }
        }
        it = it->getSiguienteServidor();
    }

    // 2) Si hay servidor con huecos libres, conectar
    if (mejorSrvConn != nullptr && maxLibres > 0) {
        mejorSrvConn->conectarJugador(j);
        mejorSrvConn->getDireccionServidor(host);
        enEspera = false;
        return true;
    }

    // 3) Si no conecta, pero hay cola con huecos, encolar
    if (mejorSrvCola != nullptr && maxCola > 0) {
        mejorSrvCola->ponerJugadorEnEspera(j);
        mejorSrvCola->getDireccionServidor(host);
        enEspera = true;
        return false;
    }

    // 4) No cabe en ningún sitio
    enEspera = false;
    return false;
}
