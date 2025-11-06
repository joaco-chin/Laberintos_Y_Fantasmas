#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED

#include "interno_matriz.h"
#include "interno_jugador.h"
#include "interno_laberinto.h"
#include "estructuras_cola.h"
#include "estructuras_pila.h"
#include "estructuras_lista.h"

#define NO_EXISTE_CAMINO -5

#define FANTASMA_VIVO 1
#define FANTASMA_MUERTO 0

typedef struct
{
    int fil;
    int col;
    int costoG;
    int costoH;
    int costoF;
    int filPadre;
    int colPadre;
    char caracterAnterior;
    tPosicion posInicial;
    int estaVivo;
}tInfoNodo;

typedef struct
{
    int fil;
    int col;
    int estaVivo;
    char caracterAnterior;
    tPosicion posInicial;
}tFantasma;

int aStarBusqueda(char** matriz, int cf, int cc, const tEntidad* inicio, const tEntidad* meta, tCola* movimientos);
void calcularMovimientosFantasmas(char** matriz, int cf, int cc, tLista* fantasmas, const tJugador* jugador, tCola* movimientos);
void limpiarFantasmas(char** matriz, tLista* listaFantasmas);
void actualizarPosicionesFantasmas(char** matriz, int cf, int cc, tCola* colaFantasmas, tJugador* jugador, tCola* movimientos);
int calcularHeuristica(const tInfoNodo* inicio, const tInfoNodo* meta);

int buscarVecinos(int cf, int cc, const tInfoNodo* nodoAct, tCola* plVecinos);
int cmpCostos(const void* a, const void* b);
int cmpPos(const void* a, const void* b);
int cmpPosFan(const void* a, const void* b);
void impFan(const void* a);
int cmpPosEntidad(const void* a, const void* b);
int cmpPosIniciales(const void* a, const void* b);
void resetearFantasma(void* info);

#endif // FANTASMA_H_INCLUDED
