#include "GestorServidores.h"
#include "Servidor.h"
#include "Common.h"

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
    // [1] — LOCALIZAR EL SERVIDOR A DESACTIVAR
    Servidor* previo = nullptr;
    Servidor* actual = primerServidor;
    bool encontrado = false;

    cout << "[~] Buscando servidor con direccion: " << dS << endl;
    while (actual != nullptr && !encontrado) {
        cadena direccionActual;
        actual->getDireccionServidor(direccionActual);
        if (strcmp(direccionActual, dS) == 0) {
            cout << "[+] Servidor encontrado." << endl;
            encontrado = true;
        } else {
            previo = actual;
            actual = actual->getSiguienteServidor();
        }
    }
    if (!encontrado) {
        cout << "[!] No se encontro ningun servidor con la direccion: " << dS << endl;
        return false;
    }

    // [2] — COMPROBAR ESTADO
    cout << "[~] Comprobando si el servidor esta activo..." << endl;
    if (!actual->estaActivo()) {
        cout << "[!] El servidor ya se encuentra INACTIVO y no se puede desconectar..." << endl;
        return false;
    }
    cout << "[+] Servidor ACTIVO, procediendo a la desconexion del mismo." << endl;

    // [3] — EXTRAER LISTAS DE JUGADORES
    int nConectados = actual->getNumJugadoresConectados();
    int nEnEspera   = actual->getNumJugadoresEnEspera();
    cout << "[~] Hay " << nConectados << " jugadores conectados. Guardando en lista..." << endl;
    cout << "[~] Hay " << nEnEspera   << " jugadores esperando. Guardando en lista..." << endl;

    Jugador* listaConectados = new Jugador[nConectados];
    Jugador* listaEnEspera   = new Jugador[nEnEspera];
    actual->exportarJugadoresConectados(listaConectados);
    actual->exportarJugadoresEnEspera(listaEnEspera);

    // [4] — DESACTIVAR SERVIDOR
    cout << "[-] Desactivando el servidor..." << endl;
    bool exitoDesactivado = actual->desactivar();
    if (!exitoDesactivado) {
        cout << "[!] No se pudo desactivar el servidor." << endl;
        delete[] listaConectados;
        delete[] listaEnEspera;
        return false;
    }
    cout << "[+] Servidor desactivado correctamente." << endl;
    cout << "[/] SERVIDOR: "
         << ( actual->estaActivo() ? "ACTIVO\n" : "INACTIVO\n" );

    cadena juegoServidor;
    actual->getNombreJuego(juegoServidor);

    // [5] — REDISTRIBUIR JUGADORES CONECTADOS (por latencia ascendente)
    cout << "[~] Organizando jugadores conectados por latencia..." << endl;
    for (int a = 0; a < nConectados - 1; ++a) {
        for (int b = a + 1; b < nConectados; ++b) {
            if (listaConectados[b].latencia < listaConectados[a].latencia) {
                std::swap(listaConectados[a], listaConectados[b]);
            }
        }
    }
    for (int i = 0; i < nConectados; ++i) {
        Jugador& j = listaConectados[i];
        cadena host;
        bool enEspera;
        bool ok = alojarJugador(j, juegoServidor, host, enEspera);
        if (ok) {
            if (!enEspera)
                cout << "[+] Jugador " << j.nombreJugador
                     << " conectado en " << host << endl;
            else
                cout << "[+] Jugador " << j.nombreJugador
                     << " en espera en " << host << endl;
        } else {
            cout << "[!] Jugador " << j.nombreJugador
                 << " expulsado del sistema." << endl;
        }
    }

    // [6] — REDISTRIBUIR JUGADORES EN ESPERA (por latencia ascendente)
    cout << "[~] Organizando jugadores en espera por latencia..." << endl;
    for (int a = 0; a < nEnEspera - 1; ++a) {
        for (int b = a + 1; b < nEnEspera; ++b) {
            if (listaEnEspera[b].latencia < listaEnEspera[a].latencia) {
                std::swap(listaEnEspera[a], listaEnEspera[b]);
            }
        }
    }
    for (int i = 0; i < nEnEspera; ++i) {
        Jugador& j = listaEnEspera[i];
        cadena host;
        bool enEspera;
        bool ok = alojarJugador(j, juegoServidor, host, enEspera);
        if (ok) {
            if (!enEspera)
                cout << "[+] Jugador " << j.nombreJugador
                     << " conectado en " << host << endl;
            else
                cout << "[+] Jugador " << j.nombreJugador
                     << " en espera en " << host << endl;
        } else {
            cout << "[!] Jugador " << j.nombreJugador
                 << " expulsado del sistema." << endl;
        }
    }

    // [7] — EXPULSAR A LOS ORIGINALES DEL SERVIDOR INACTIVO
    for (int i = 0; i < nEnEspera; ++i) {
        actual->expulsarJugador(listaEnEspera[i].nombreJugador);
    }
    for (int i = 0; i < nConectados; ++i) {
        actual->expulsarJugador(listaConectados[i].nombreJugador);
    }

    // [8] — LIMPIEZA DE RECURSOS
    delete[] listaConectados;
    delete[] listaEnEspera;

    cout << "[OK] Todos los jugadores han sido redistribuidos y el servidor esta INACTIVO y vacio." << endl;
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
    cout << "[~] Comprobando que el servidor este INACTIVO o en MANTENIMIENTO..." << endl;
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
    delete actual;

    numServidores--;
    cout << "[OK] Servidor eliminado correctamente. Quedan " << numServidores << " servidores." << endl;

    return true;
}

bool GestorServidores::alojarJugador(Jugador j, cadena nomJuego, cadena host, bool &enEspera) {

    Servidor* it           = primerServidor;
    Servidor* mejorSrvConn = nullptr;
    int       maxLibres    = -1;
    Servidor* mejorSrvCola = nullptr;
    int       maxCola      = -1;
    cadena    juegoSrv;

    // 1) Recorremos todos los servidores activos que ejecuten nomJuego
    while (it != nullptr) {
        it->getNombreJuego(juegoSrv);
        if (it->estaActivo() && strcmp(juegoSrv, nomJuego) == 0) {
            // Huecos libres para conexión
            int libres = it->getMaxJugadoresConectados()
                       - it->getNumJugadoresConectados();
            if (libres > maxLibres) {
                maxLibres    = libres;
                mejorSrvConn = it;
            }
            // Huecos libres en la cola
            int colaLibres = it->getMaxJugadoresEnEspera()
                           - it->getNumJugadoresEnEspera();
            if (colaLibres > maxCola) {
                maxCola      = colaLibres;
                mejorSrvCola = it;
            }
        }
        it = it->getSiguienteServidor();
    }

    // 2) Si encontramos servidor con huecos libres, lo conectamos
    if (mejorSrvConn != nullptr && maxLibres > 0) {
        mejorSrvConn->conectarJugador(j);
        mejorSrvConn->getDireccionServidor(host);
        enEspera = false;
        return true;
    }

    // 3) Si no hay hueco de conexión pero sí en cola, encolamos
    if (mejorSrvCola != nullptr && maxCola > 0) {
        mejorSrvCola->ponerJugadorEnEspera(j);
        mejorSrvCola->getDireccionServidor(host);
        enEspera = true;
        return true;
    }

    // 4) No cabe en ningún sitio
    enEspera = false;
    return false;
}

bool GestorServidores::expulsarJugador(cadena nJ, cadena host) {
    // 1) Recorrer todos los servidores en busca del jugador
    Servidor* it = primerServidor;
    cout << "[~] Buscando jugador \"" << nJ << "\" en el sistema..." << endl;

    while (it != nullptr) {
        // 2) Intentar expulsarlo de este servidor (conectados o cola)
        if (it->expulsarJugador(nJ)) {
            // 3) Si se expulsó, devolvemos su host
            it->getDireccionServidor(host);
            cout << "[OK] Jugador \"" << nJ
                 << "\" expulsado del servidor " << host << endl;
            return true;
        }
        it = it->getSiguienteServidor();
    }

    // 4) Si no estaba en ningún servidor ni cola, devolvemos false
    cout << "[!] Jugador \"" << nJ << "\" no se encontro en el sistema." << endl;
    return false;
}

int GestorServidores::getPosicionServidor(cadena dS) {
    Servidor* actual = primerServidor;
    int pos = 1;

    cout << "[~] Buscando posicion del servidor con direccion: " << dS << endl;
    while (actual != nullptr) {
        cadena dirActual;
        actual->getDireccionServidor(dirActual);
        if (strcmp(dirActual, dS) == 0) {
            cout << "[OK] Servidor encontrado en posicion: " << pos << endl;
            return pos;
        }
        actual = actual->getSiguienteServidor();
        ++pos;
    }

    cout << "[!] No existe ningun servidor con direccion: " << dS << endl;
    return -1;
}

void GestorServidores::mostrarInformacionServidores(int pos) {
    int total = numServidores;

    // Caso: mostrar todos
    if (pos == -1) {
        if (total == 0) {
            cout << "[!] No hay servidores desplegados." << endl;
            cout << endl;
            return;
        }

        cout << "[~] Mostrando informacion de los " << total << " servidores:" << endl;
        cout << endl;

        Servidor* it = primerServidor;
        int idx = 1;
        while (it != nullptr) {
            cout << endl;
            cout << "------------------------------------- Servidor #" << idx << " -------------------------------------" << endl;
            it->mostrarInformacion();  // muestre estado, jugadores, etc.
            if (it->estaActivo()) {
                it->mostrarJugadoresConectados();
                it->mostrarJugadoresEnEspera();
            }
            cout << endl;
            it = it->getSiguienteServidor();
            ++idx;
        }
        return;
    }

    // Caso: posición fuera de rango
    if (pos < 1 || pos > total) {
        cout << "[!] Posicion " << pos
             << " fuera de rango (1–" << total << ")." << endl;
        return;
    }

    // Caso: mostrar un único servidor
    Servidor* it = primerServidor;
    int idx = 1;
    while (idx < pos) {
        it = it->getSiguienteServidor();
        ++idx;
    }

    it->mostrarInformacion();
    if (it->estaActivo()) {
        it->mostrarJugadoresConectados();
        it->mostrarJugadoresEnEspera();
    }
}

bool GestorServidores::jugadorConectado(cadena nJ, cadena dS) {
    // 1) Localizar el servidor
    Servidor* actual = primerServidor;
    while (actual != nullptr) {
        cadena dirActual;
        actual->getDireccionServidor(dirActual);
        if (strcmp(dirActual, dS) == 0) {
            break;
        }
        actual = actual->getSiguienteServidor();
    }
    if (actual == nullptr) {
        // No existe ese servidor
        return false;
    }

    // 2) Obtener número de jugadores conectados en ese servidor
    int nCon = actual->getNumJugadoresConectados();
    if (nCon == 0) {
        return false;
    }

    // 3) Volcar la lista de conectados y buscar el nombre
    Jugador* lista = new Jugador[nCon];
    actual->exportarJugadoresConectados(lista);
    bool encontrado = false;
    for (int i = 0; i < nCon; ++i) {
        if (strcmp(lista[i].nombreJugador, nJ) == 0) {
            encontrado = true;
            break;
        }
    }
    delete[] lista;
    return encontrado;
}

bool GestorServidores::jugadorEnEspera(cadena nJ, cadena dS) {
    // 1) Buscar el servidor por dirección
    Servidor* actual = primerServidor;
    while (actual != nullptr) {
        cadena dir;
        actual->getDireccionServidor(dir);
        if (strcmp(dir, dS) == 0) {
            break;
        }
        actual = actual->getSiguienteServidor();
    }
    if (actual == nullptr) {
        // No existe ese servidor
        return false;
    }

    // 2) Volcar la cola de espera y buscar el nombre
    int nEnEspera = actual->getNumJugadoresEnEspera();
    if (nEnEspera == 0) {
        return false;
    }

    Jugador* cola = new Jugador[nEnEspera];
    actual->exportarJugadoresEnEspera(cola);

    bool encontrado = false;
    for (int i = 0; i < nEnEspera; ++i) {
        if (strcmp(cola[i].nombreJugador, nJ) == 0) {
            encontrado = true;
            break;
        }
    }
    delete[] cola;
    return encontrado;
}

bool GestorServidores::jugadorConectado(cadena nJ) {
    Servidor* it = primerServidor;
    cadena     dir;

    while (it != nullptr) {
        if (it->estaActivo()) {
            it->getDireccionServidor(dir);
            if (jugadorConectado(nJ, dir)) {
                return true;
            }
        }
        it = it->getSiguienteServidor();
    }

    return false;
}

bool GestorServidores::jugadorEnEspera(cadena nJ) {
    Servidor* it = primerServidor;
    cadena     dir;

    while (it != nullptr) {
        if (it->estaActivo()) {
            it->getDireccionServidor(dir);
            if (jugadorEnEspera(nJ, dir)) {
                return true;
            }
        }
        it = it->getSiguienteServidor();
    }

    return false;
}

Servidor* GestorServidores::getServidorMasPuntuacion(cadena nombreJuego) {
    Servidor *iter = primerServidor;
    Servidor *mejorServidor = nullptr;
    int mejorPuntuacion = -1;
    cadena nombreJuegoIter;

    cout << "[~] Buscando el servidor con mas puntuacion" << endl;

    while(iter != nullptr) {
        iter->getNombreJuego(nombreJuegoIter);
        if(iter->estaActivo() && strcmp(nombreJuego, nombreJuegoIter) == 0) {
            int jugadoresConectados = iter->getNumJugadoresConectados();
            int suma = 0;

            if(jugadoresConectados > 0) {
                Jugador *lista = new Jugador[jugadoresConectados];
                iter->exportarJugadoresConectados(lista);
                for(int i = 0; i < jugadoresConectados; i++) {
                    suma += lista[i].puntuacion;
                }
                delete[] lista;
            }

            if(suma > mejorPuntuacion) {
                mejorPuntuacion = suma;
                mejorServidor = iter;
            }
        }
        iter = iter->getSiguienteServidor();
    }

    if (mejorServidor != nullptr) {
        cout << "[OK] Servidor encontrado con puntuación total: "
             << mejorPuntuacion << endl;
    } else {
        cout << "[!] No hay servidores activos para ese juego." << endl;
    }
    return mejorServidor;
}
