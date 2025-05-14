# Práctica 2: Sistema de Gestión de Servidores de Juegos

Este proyecto implementa un sistema de gestión de servidores de juegos que permite administrar servidores y la conexión de jugadores a los mismos. La aplicación está desarrollada en C++ como parte de la práctica 2 de la asignatura de Estructura de Datos I.

## Características

- Creación y configuración de servidores de juegos
- Gestión de conexiones de jugadores
- Sistema de cola para jugadores cuando el servidor está lleno
- Ordenamiento automático de jugadores por puntuación
- Visualización de información de servidores y jugadores

## Estructura del Proyecto

El proyecto está organizado en los siguientes directorios:

- **include/**: Contiene los archivos de cabecera (.h)
  - `Common.h`: Define tipos comunes y estructuras
  - `Servidor.h`: Definición de la clase Servidor
  - `TADLista.h`: Implementación del TAD Lista
  - `TADCola.h`: Implementación del TAD Cola
  
- **src/**: Contiene los archivos de implementación (.cpp)
  - `Servidor.cpp`: Implementación de la clase Servidor
  - `TADLista.cpp`: Implementación del TAD Lista
  - `TADCola.cpp`: Implementación del TAD Cola

- **bin/**: Directorio donde se generan los ejecutables compilados

## Clases Principales

### Clase Servidor

Representa un servidor de juegos con las siguientes propiedades:
- Dirección IP
- Nombre del juego
- ID único
- Capacidad máxima de jugadores conectados
- Capacidad máxima de jugadores en espera
- Puerto
- Localización geográfica
- Estado (activo, inactivo, mantenimiento)

### Estructura Jugador

Representa a un jugador con:
- Nombre
- ID único
- Estado (activo o inactivo)
- Latencia de conexión
- Puntuación
- País

### TADs Implementados

- **Lista**: Implementada como una estructura dinámica para gestionar los jugadores conectados
- **Cola**: Implementada para gestionar la lista de espera de jugadores

## Funcionalidades

El sistema permite:
1. Crear y configurar servidores
2. Conectar jugadores a un servidor
3. Poner jugadores en espera cuando el servidor está lleno
4. Ordenar automáticamente los jugadores por puntuación
5. Mostrar información detallada sobre servidores y jugadores
6. Expulsar jugadores
7. Cambiar el estado del servidor

## Compilación y Ejecución

El proyecto utiliza Code::Blocks como entorno de desarrollo. Para compilar:

1. Abrir el archivo `EDI-Practica-02.cbp` en Code::Blocks
2. Seleccionar "Build" o presionar F9
3. Para ejecutar, seleccionar "Run" o presionar F10

## Ejemplo de Uso

El archivo `main.cpp` incluye un ejemplo que:
1. Crea un servidor con configuración específica
2. Crea varios jugadores de prueba
3. Intenta conectar a los jugadores al servidor
4. Muestra información sobre jugadores conectados y en espera

```cpp
Servidor server1("192.168.18.1", "Minecraft", 1, 5, 4, 25565, "Espana");
// Crear jugadores y conectarlos...
server1.mostrarJugadoresConectados();
server1.mostrarJugadoresEnEspera();
```

## Autores

Desarrollo como parte de la Práctica 2 de Estructura de Datos I. 