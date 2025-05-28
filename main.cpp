#include <iostream>
#include <cstring>
#include "Common.h"
#include "Servidor.h"
#include "GestorServidores.h"

using namespace std;

void menuPrincipal(GestorServidores &gestor) {

    int opc;

    do{
        system("cls");
        cout << "GESTOR DE SERVIDORES v1.0" << endl;
        cout << "=======================================================================" << endl;
        cout << "1. Mostrar Servidor" << endl;
        cout << "2. Crear Servidor" << endl;
        cout << "3. Eliminar Servidor" << endl;
        cout << "4. Activar Servidor" << endl;
        cout << "5. Desactivar Servidor" << endl;
        cout << "6. Programar Mantenimiento de Servidor" << endl;
        cout << "7. Conectar Jugador" << endl;
        cout << "8. Expulsar Jugador" << endl;
        cout << "9. Salir" << endl;
        cout << endl;
        cout << "Seleccione una Opcion: "; cin >> opc; cin.ignore();

        switch (opc) {

            case 1:{

                system("cls");
                cout << endl;
                cout << "=== MOSTRAR SERVIDOR ===" << endl;
                cout << endl;


                cadena hostame;
                cout << "Introduce la direccion/hostame del servidor (o -1 para mostrarlos todos): ";
                cin.getline(hostame, 50);
                cout << endl;

                if(strcmp(hostame, "-1") == 0) {
                    gestor.mostrarInformacionServidores(-1);
                } else {
                    int pos = gestor.getPosicionServidor(hostame);

                    if (pos != -1) {
                        gestor.mostrarInformacionServidores(pos);
                    }
                }


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 2: {

                system("cls");
                cout << endl;
                cout << "=== CREAR SERVIDOR ===" << endl;
                cout << endl;


                cadena dS, nJ, lG;
                int id, puerto, mxL, mxC;

                cout << "Direccion/Hostame: ";
                cin.getline(dS, 50);

                cout << "Codigo Identificador (ID): ";
                cin >> id;
                cin.ignore();

                cout << "Nombre del juego instalado: ";
                cin.getline(nJ, 50);

                cout << "Puerto de escucha: ";
                cin >> puerto;
                cin.ignore();

                cout << "Maximo jugadores conectados: ";
                cin >> mxL;
                cin.ignore();

                cout << "Maximo jugadores en espera: ";
                cin >> mxC;
                cin.ignore();

                cout << "Localizacion geografica (pais): ";
                cin.getline(lG, 50);

                cout << endl;

                cout << "----- PROCESO DE DESPLIEGUE -----" << endl;
                gestor.desplegarServidor(dS, nJ, id, mxL, mxC, puerto, lG);
                cout << "------------------------------------" << endl;


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 3: {

                system("cls");
                cout << endl;
                cout << "=== ELIMINAR SERVIDOR ===" << endl;
                cout << endl;


                cadena dS;
                cout << "Introduce la direccion/hostame del servidor para eliminarlo: ";
                cin.getline(dS, 50);
                cout << endl;

                cout << "----- PROCESO DE DESACTIVACION -----" << endl;
                gestor.desconectarServidor(dS);
                cout << "------------------------------------" << endl;

                cout << endl;

                cout << "----- PROCESO DE ELIMINACION -----" << endl;
                gestor.eliminarServidor(dS);
                cout << "------------------------------------" << endl;


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 4: {

                system("cls");
                cout << endl;
                cout << "=== ACTIVAR SERVIDOR ===" << endl;
                cout << endl;


                cadena dS;
                cout << "Introduce la direccion/hostame del servidor para eliminarlo: ";
                cin.getline(dS, 50);
                cout << endl;

                cout << "----- PROCESO DE ACTIVACION -----" << endl;
                gestor.conectarServidor(dS);
                cout << "------------------------------------" << endl;


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 5: {

                system("cls");
                cout << endl;
                cout << "=== DESACTIVAR SERVIDOR ===" << endl;
                cout << endl;


                cadena dS;
                cout << "Introduce la direccion/hostame del servidor para eliminarlo: ";
                cin.getline(dS, 50);
                cout << endl;

                cout << "----- PROCESO DE DESACTIVACION -----" << endl;
                gestor.desconectarServidor(dS);
                cout << "------------------------------------" << endl;


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 6: {

                system("cls");
                cout << endl;
                cout << "=== PROGRAMAR MANTENIMIENTO DEL SERVIDOR ===" << endl;
                cout << endl;


                cadena dS;
                cout << "Introduce la direccion/hostame del servidor para eliminarlo: ";
                cin.getline(dS, 50);
                cout << endl;

                cout << "----- PROCESO DE MANTENIMIENTO -----" << endl;
                gestor.realizarMantenimiento(dS);
                cout << "------------------------------------" << endl;


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 7: {

                system("cls");
                cout << endl;
                cout << "=== CONECTAR UN NUEVO JUGADOR ===" << endl;
                cout << endl;


                cadena nombre;
                cout << "Nombre del jugador: ";
                cin.getline(nombre, 50);

                bool estaConectado = false;

                if(gestor.jugadorConectado(nombre)) {
                    estaConectado = true;
                }

                if(gestor.jugadorEnEspera(nombre)) {
                    estaConectado = true;
                }

                if(estaConectado) {
                    cout << endl;
                    cout << "[!] Ya existe un jugador con ese nombre, vuelve a intentarlo..." << endl;
                    cout << endl;
                    system("pause");
                    system("cls");
                    break;
                }

                int id;
                cout << "Identificador unico (ID): ";
                cin >> id; cin.ignore();

                cadena pais;
                cout << "Pais desde el que se conecta: ";
                cin.getline(pais, 50);

                cadena juego;
                cout << "Nombre del juego al que quiere jugar: ";
                cin.getline(juego, 50);

                int latencia = rand() % 500 + 1;
                int puntuacion = rand() % 100000;

                Jugador j;
                strcpy(j.nombreJugador, nombre);
                j.ID = id;
                strcpy(j.pais, pais);
                j.latencia = latencia;
                j.puntuacion = puntuacion;
                j.activo = false;

                cadena host;
                bool enEspera;
                bool ok = gestor.alojarJugador(j, juego, host, enEspera);

                // 6) Mostrar resultado
                if (ok && !enEspera) {
                    cout << "\n[OK] El jugador \"" << nombre << "\" ha sido conectado en el servidor: " << host << "\n";
                }
                else if (ok && enEspera) {
                    cout << "\n[OK] El jugador \"" << nombre << "\" ha sido puesto en espera en el servidor: " << host << "\n";
                }
                else {
                    cout << "\n[!] ERROR: No hay ningun servidor activo para \"" << juego << "\" con espacio disponible. Jugador expulsado.\n";
                }


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 8: {

                system("cls");
                cout << endl;
                cout << "=== CONECTAR UN NUEVO JUGADOR ===" << endl;
                cout << endl;


                cadena nombre;
                cout << "Nombre (nick) del jugador a expulsar: ";
                cin.getline(nombre, 50);
                cout << endl;

                // 2) Intentar expulsar
                cadena host;
                cout << "----- PROCESO DE ACTIVACION -----" << endl;
                gestor.expulsarJugador(nombre, host);
                cout << "------------------------------------" << endl;


                cout << endl;
                system("pause");
                system("cls");
                break;
            }

            case 9: {

                system("cls");
                cout << endl;
                cout << "=== SALIENDO DEL PROGRAMAM ===" << endl;
                break;
            }

            default: {

                cout << "[!] ERROR: Escriba un valor valido..." << endl;
                break;
            }

        }
    } while (opc != 9);
}

int main()
{
    GestorServidores gestor;

    menuPrincipal(gestor);
    return 0;
}
