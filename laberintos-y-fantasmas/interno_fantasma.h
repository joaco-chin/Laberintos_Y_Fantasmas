#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED

#include "interno_matriz.h"
#include "interno_jugador.h"
#include "estructuras_cola.h"
#include "estructuras_pila.h"
#include "estructuras_lista.h"

#define HAY_MOVIMIENTO 1
#define NO_HAY_MOVIMIENTO 0

#define NO_EXISTE_CAMINO -5

typedef struct
{
    int fil;
    int col;
    int costoG;
    int costoH;
    int costoF;
    int filPadre;
    int colPadre;
}tInfoNodo;

typedef struct
{
    char dibujo;
    int fil;
    int col;
    int estaVivo;
}tFantasma;

int aStarBusqueda(char** matriz, int cf, int cc, const tInfoNodo* inicio, const tInfoNodo* meta, tCola* movimientos);
int calcularMovimientosFantasmas(char** matriz, int cf, int cc, tFantasma* fantasma, int ce, tJugador* jugador, tCola* movimientos);

int calcularHeuristica(const tInfoNodo* inicio, const tInfoNodo* meta);
int buscarVecinos(int cf, int cc, const tInfoNodo* nodoAct, tCola* plVecinos);
int cmpCostos(const void* a, const void* b);
int cmpPos(const void* a, const void* b);

#endif // FANTASMA_H_INCLUDED
