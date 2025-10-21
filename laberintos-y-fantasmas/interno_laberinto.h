#ifndef INTERNO_LABERINTO_H_INCLUDED
#define INTERNO_LABERINTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interno_matriz.h"
#include "codigosRet.h"
#include "generico.h"
#include "estructuras_lista.h"
#include "estructuras_cola.h"

// poner esto en codigosRet.h??
#define VERDADERO 1
#define FALSO 0

// esto lo use solo para probar
#define FILAS 15
#define COLUMNAS 15
#define MAX_FANTASMAS 5
#define MAX_PREMIOS 5
#define MAX_VIDAS_EXTRA 5
#define CANT_MAX_DIREC 4
//-----

#define PARED '#'
#define PARED_TEMPORAL '$'
#define PARED_RANGO_ENTRADA '+'
#define CAMINO ' '
#define ENTRADA 'E'
#define SALIDA 'S'
#define FANTASMA 'F'
#define PREMIO 'P'
#define VIDA_EXTRA 'V'

typedef struct
{
    int fila;
    int columna;
} tPosicion;

tPosicion elegirYEliminarPosicionLista(tLista *lista, int *cantidad);
int llenarListaPosicionesLibresParedes(char **matriz, int filas, int columnas, tLista *lista);
int llenarListaPosicionesLibres(char **matriz, int filas, int columnas, tLista *lista);
int colocarCaracteresEnPosicionesAleatorias(char **matriz, int filas, int columnas, char caracter, int cantidadCar, tCola* colaFantasmas);
void generarEntradaYSalida(char **matriz, int filas, int columnas, tPosicion *posEnt, tPosicion *posSal);
void evitarObstrucciones(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal, int maxBloquesPorPared);
void colocarCaracterEnEsquinasDePosicion(char **matriz, int filas, int columnas, tPosicion pos, char caracter);
void actualizarCaracterAlrededorDePosicion(char **matriz, int filas, int columnas, tPosicion pos, char car, char nuevoCar, int rango);
void barajarOrdenDirecciones(int *ordenDirecciones, int n);
void colocarParedTemporalEnPosicion(char **matriz, int filas, int columnas, tPosicion posActual, int dirElegida, int cantBloquesMax);
int hayCaracterAlrededorDePosicion(char **matriz, int filas, int columnas, int i, int j, char objetivo);
void generarParedesInternas(char **matriz, int filas, int columnas, int maxBloquesPorPared);
void generarParedesLimite(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal);
void colocarCaracteresEspeciales(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios, unsigned *vidasExtra, tCola* colaFantasmas);
void generarLaberintoAleatorio(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios, unsigned *vidasExtra, tCola* colaFantasmas);

// funciones para poner en matriz.c
void inicializarMatrizCaracter(char **matriz, int filas, int columnas, char caracter);

// funciones de lista REVISAR
int listaObtenerDatoPorPosicionREVISAR(tLista *lista, int posicion, void *dato, size_t tamDato);
int listaEliminarNodoPorPosicionREVISAR(tLista *lista, int posicion, void *dato, size_t tamDato);
int listaPonerAlFinalREVISAR(tLista *pl, const void *dato, size_t tamDato);
void listaVaciarREVISAR(tLista *pl);

// funcion para poner en principal_archivo
void escribirMatrizEnArchivoTxt(char **matriz, const char *nomArch, int filas, int columnas);

#endif // INTERNO_LABERINTO_H_INCLUDED
