#ifndef INTERNO_LABERINTO_H_INCLUDED
#define INTERNO_LABERINTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interno_matriz.h"
#include "..\comun\codigosRet.h"
#include "generico.h"
#include "estructuras_lista.h"
#include "estructuras_cola.h"

#define CANT_MAX_DIREC 4

#define PARED '#'
#define PARED_TEMPORAL '$'
#define PARED_RANGO_ENTRADA '+'
#define CAMINO '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define FANTASMA 'F'
#define PREMIO 'P'
#define VIDA_EXTRA 'V'

typedef struct
{
    int fila;
    int columna;
}tPosicion;

typedef struct
{
    int y;
    int x;
    char caracterEnt;
    char caracterFondo;
    tPosicion posInicial;
    int estaVivo;
}tEntidad;

tPosicion elegirYEliminarPosicionLista(tLista *lista, int *cantidad);
int llenarListaPosicionesLibresParedes(char **matriz, int filas, int columnas, tLista *lista);
int llenarListaPosicionesLibres(char **matriz, int filas, int columnas, tLista *lista);
int colocarCaracteresEnPosicionesAleatorias(char **matriz, int filas, int columnas, char caracter, int cantidadCar, tLista* fantasmas);
void generarEntradaYSalida(char **matriz, int filas, int columnas, tPosicion *posEnt, tPosicion *posSal);
void evitarObstruccionesEsquinas(char **matriz, int filas, int columnas, int maxBloquesPorPared);
void evitarObstruccionesEntradaSalida(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal, int maxBloquesPorPared);
void colocarCaracterEnEsquinasDePosicion(char **matriz, int filas, int columnas, tPosicion pos, char caracter);
void actualizarCaracterAlrededorDePosicion(char **matriz, int filas, int columnas, tPosicion pos, char car, char nuevoCar, int rango);
void barajarOrdenDirecciones(int *ordenDirecciones, int n);
void colocarParedTemporalEnPosicion(char **matriz, int filas, int columnas, tPosicion posActual, int dirElegida, int cantBloquesMax);
int hayCaracterAlrededorDePosicion(char **matriz, int filas, int columnas, int i, int j, char objetivo);
void generarParedesInternas(char **matriz, int filas, int columnas, int maxBloquesPorPared);
void generarParedesLimite(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal);
void colocarCaracteresEspeciales(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios, unsigned *vidasExtra, tLista* listaFantasmas);
void generarLaberintoAleatorio(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios,
unsigned *vidasExtra, tLista* listaFantasmas, tPosicion entradaYSalida[]);

#endif // INTERNO_LABERINTO_H_INCLUDED
