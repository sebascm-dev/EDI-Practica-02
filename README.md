# Práctica 2: Sistema de Gestión de Servidores de Juegos

Este proyecto implementa un sistema completo de gestión de servidores de juegos, permitiendo administrar la creación, configuración y mantenimiento de servidores, así como la conexión y gestión de jugadores. La aplicación está desarrollada en C++ como parte de la práctica 2 de la asignatura de Estructura de Datos I.

## Características Principales

- Gestión completa del ciclo de vida de servidores (creación, activación, desactivación, mantenimiento y eliminación)
- Administración de conexiones de jugadores a servidores específicos
- Sistema de cola para jugadores cuando el servidor alcanza su capacidad máxima
- Ordenamiento automático de jugadores por puntuación
- Expulsión de jugadores de servidores
- Interfaz de usuario basada en menús para la gestión del sistema
- Visualización detallada de información de servidores y jugadores

## Estructura del Proyecto

El proyecto está organizado en los siguientes directorios:

- **include/**: Contiene los archivos de cabecera (.h)
  - `Common.h`: Define tipos comunes y la estructura Jugador
  - `Servidor.h`: Definición de la clase Servidor que gestiona un servidor individual
  - `GestorServidores.h`: Definición de la clase principal que gestiona múltiples servidores
  - `TADLista.h`: Implementación del TAD Lista para almacenar jugadores conectados
  - `TADCola.h`: Implementación del TAD Cola para gestionar jugadores en espera
  
- **src/**: Contiene los archivos de implementación (.cpp)
  - `Servidor.cpp`: Implementación de la clase Servidor
  - `GestorServidores.cpp`: Implementación de la clase GestorServidores
  - `TADLista.cpp`: Implementación del TAD Lista
  - `TADCola.cpp`: Implementación del TAD Cola
  - `main.cpp`: Punto de entrada de la aplicación con el menú de usuario

- **bin/**: Directorio donde se generan los ejecutables compilados
- **obj/**: Directorio para archivos objeto intermedios

## Clases y Estructuras

### Estructura Jugador (Common.h)

Representa a un jugador con los siguientes atributos:
- `nombreJugador`: Nombre del jugador
- `ID`: Identificador único
- `activo`: Estado del jugador (activo o inactivo)
- `latencia`: Latencia de conexión al servidor
- `puntuacion`: Puntuación del jugador
- `pais`: País de origen del jugador

### Clase Servidor

Representa un servidor de juegos individual con las siguientes propiedades:
- `direccionServidor`: IP o nombre de host del servidor
- `nombreJuego`: Nombre del juego instalado en el servidor
- `id`: Identificador único del servidor
- `maxJugadoresConectados`: Capacidad máxima de jugadores simultáneos
- `maxJugadoresEnEspera`: Capacidad máxima de la cola de espera
- `jugadoresConectados`: Lista de jugadores actualmente conectados
- `jugadoresEnEspera`: Cola de jugadores en espera
- `estado`: Estado actual del servidor (activo, inactivo, mantenimiento)
- `puerto`: Puerto de escucha del servidor
- `localizacionGeografica`: País donde se encuentra el servidor físicamente

### Clase GestorServidores

Clase principal que gestiona múltiples servidores, implementando funcionalidades como:
- Despliegue de nuevos servidores
- Conexión/desconexión de servidores
- Mantenimiento programado
- Eliminación de servidores
- Gestión de conexiones de jugadores
- Expulsión de jugadores
- Consulta de información sobre servidores

### TADs Implementados

- **Lista**: Estructura de datos que implementa una lista dinámica para gestionar los jugadores conectados
  - Permite añadir, eliminar, buscar y modificar jugadores
  - Implementa métodos para añadir elementos por ambos extremos

- **Cola**: Estructura de datos tipo FIFO (First In, First Out) para gestionar la lista de espera de jugadores
  - Implementa operaciones básicas como encolar, desencolar, consultar el primer elemento

## Funcionalidades del Sistema

El sistema permite realizar las siguientes operaciones a través de un menú interactivo:

1. **Mostrar Información de Servidores**: Visualizar detalles de un servidor específico o de todos los servidores
2. **Crear Servidor**: Desplegar un nuevo servidor con configuración personalizada
3. **Eliminar Servidor**: Dar de baja un servidor existente
4. **Activar Servidor**: Poner en marcha un servidor inactivo
5. **Desactivar Servidor**: Apagar temporalmente un servidor
6. **Programar Mantenimiento**: Establecer un servidor en modo mantenimiento
7. **Conectar Jugador**: Añadir un jugador a un servidor (o a su cola de espera si está lleno)
8. **Expulsar Jugador**: Eliminar un jugador de un servidor

## Compilación y Ejecución

El proyecto utiliza Code::Blocks como entorno de desarrollo. Para compilar:

1. Abrir el archivo `EDI-Practica-02.cbp` en Code::Blocks
2. Seleccionar "Build" o presionar F9
3. Para ejecutar, seleccionar "Run" o presionar F10

Alternativamente, se puede compilar manualmente con g++:

```bash
g++ -I./include -c ./src/Servidor.cpp -o ./obj/Servidor.o
g++ -I./include -c ./src/GestorServidores.cpp -o ./obj/GestorServidores.o
g++ -I./include -c ./src/TADLista.cpp -o ./obj/TADLista.o
g++ -I./include -c ./src/TADCola.cpp -o ./obj/TADCola.o
g++ -I./include -c ./main.cpp -o ./obj/main.o
g++ ./obj/Servidor.o ./obj/GestorServidores.o ./obj/TADLista.o ./obj/TADCola.o ./obj/main.o -o ./bin/EDI-Practica-02
```

## Ejemplo de Uso

El sistema cuenta con una interfaz de usuario que permite realizar todas las operaciones mencionadas. A continuación, se muestra un ejemplo de código para crear y usar un servidor:

```cpp
// Crear una instancia del gestor de servidores
GestorServidores gestor;

// Desplegar un nuevo servidor
gestor.desplegarServidor("192.168.1.100", "Minecraft", 1, 10, 5, 25565, "España");

// Crear un jugador
Jugador j1;
strcpy(j1.nombreJugador, "Player1");
j1.ID = 1001;
j1.activo = true;
j1.latencia = 50;
j1.puntuacion = 1500;
strcpy(j1.pais, "España");

// Conectar el jugador al servidor
bool enEspera = false;
gestor.alojarJugador(j1, "Minecraft", "192.168.1.100", enEspera);

// Mostrar información del servidor
gestor.mostrarInformacionServidores(0);
```

## Requisitos

- Sistema operativo: Windows (probado en Windows 10)
- Compilador: MinGW con soporte para C++11 o superior
- IDE: Code::Blocks (opcional)

## Autor

Desarrollado como parte de la Práctica 2 de la asignatura Estructura de Datos I. 