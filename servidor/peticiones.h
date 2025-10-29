#ifndef PETICIONES_H_INCLUDED
#define PETICIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor.h"
#include "estructuras_arbol.h"

#define ARCHIVO_INDICE "indice_jugadores.bin"
#define ARCHIVO_JUGADORES "jugadores.bin"
#define ARCHIVO_PARTIDAS "partidas.bin"
#define MAX_NOMBRE_JUGADOR 50
#define MAX_DIFICULTAD 20

typedef struct
{
    char nombre[MAX_NOMBRE_JUGADOR];
    unsigned id;
    unsigned desplazamiento;
} tJugadorArbol;

typedef struct
{
    unsigned id;
    char nombre[MAX_NOMBRE_JUGADOR];
    unsigned puntuacion;
    unsigned partidasJugadas;
} tJugadorArchivo;

typedef struct
{
    unsigned idPartida;
    unsigned idJugador;
    char nombre[MAX_NOMBRE_JUGADOR];
    unsigned puntuacionObtenida;
    unsigned cantMovimientos;
    char dificultad[MAX_DIFICULTAD];
} tPartidaArchivo;

void escribirJugadorEnArchivo(const tJugadorArbol *nuevoJugador, FILE *archivoJugadores);
int procesarNombreJugador(SOCKET cliente, char *buffer, char *respuesta, tArbol *arbolJugadores, FILE *archivoJugadores);
void escribirPartidaEnArchivo(tJugadorArchivo *jugador, unsigned puntos, unsigned cantMovimientos, const char *dificultad, FILE *archivoPartidas);
int actualizarPuntuacionJugador(const char *peticion, FILE *archivoJugadores, unsigned desplazamiento, FILE *archivoPartidas);
void procesarPeticion(const char *peticion, char *respuesta, FILE *archivoJugadores, unsigned desplazamiento, FILE *archivoPartidas);

// funciones auxiliares de juegadores
int compararNombresJugadores(const void *nombre, const void *estructuraJugador);
void accionMostrarNodoJugadorArbol(void *dato, size_t tamDato, unsigned n, void *params);
int compararIDJugadores(const void *jugadorArbolA, const void *jugadorArbolB);

#endif // PETICIONES_H_INCLUDED
