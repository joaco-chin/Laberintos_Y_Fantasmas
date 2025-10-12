#ifndef FANTASMA_H_INCLUDED
#define FANTASMA_H_INCLUDED

#include "interno_matriz.h"
#include "interno_jugador.h"

#define HAY_MOVIMIENTO 1
#define NO_HAY_MOVIMIENTO 0

typedef struct
{
    int fil;
    int col;
    int costoG;
    int costoH;
    int costoF;
}tInfoNodo;

#endif // FANTASMA_H_INCLUDED
