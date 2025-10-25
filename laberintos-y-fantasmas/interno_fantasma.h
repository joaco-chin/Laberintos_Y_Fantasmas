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

void buscarFantasmasEnMatriz(char** matriz, int cf, int cc, tFantasma* vecFantasma, int ce);
int aStarBusqueda(char** matriz, int cf, int cc, const tFantasma* inicio, const tJugador* meta, tCola* movimientos);
//void calcularMovimientosFantasmas(char** matriz, int cf, int cc, tLista* listaDeFantasmas, tJugador* jugador, tCola* movimientos);
void calcularMovimientosFantasmas(char** matriz, int cf, int cc, tCola* colaFantasmas, const tJugador* jugador, tCola* movimientos);
//void actualizarPosicionFantasma(char** matriz, int cf, int cc, tFantasma* vecFantasma, tInfoNodo* movimiento);
void actualizarPosicionesFantasmas(char** matriz, int cf, int cc, tCola* colaFantasmas, tJugador* jugador, tCola* movimientos);
int calcularHeuristica(const tInfoNodo* inicio, const tInfoNodo* meta);

int buscarVecinos(int cf, int cc, const tInfoNodo* nodoAct, tCola* plVecinos);
int cmpCostos(const void* a, const void* b);
int cmpPos(const void* a, const void* b);
int cmpPosFan(const void* a, const void* b);
//int cmpOrden(const void* a, const void* b);

void impFan(const void* a);
void _mostrarFantasmas_TEST(const tFantasma* vecFantasma, int ce); // Para testear si los fantasmas se buscan de forma correcta

#endif // FANTASMA_H_INCLUDED
