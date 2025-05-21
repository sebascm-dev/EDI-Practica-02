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
    // [X] - VALIDACION DE DUPLICADOS
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

    // [] - CREACION DEL NUEVO NODO
    // [] - DETERMINACION DEL PUNTO DE INSERCCION
    // [] - INSERCCION EN LA LISTA ENLAZADA
    // [] - ACTUALIZACION DE CONTADOR
}
