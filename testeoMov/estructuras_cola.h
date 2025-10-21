#ifndef ESTRUCTURAS_COLA_H_INCLUDED
#define ESTRUCTURAS_COLA_H_INCLUDED

#include "estructuras_comun.h"

#define COLA_VACIA -3
#define COLA_CON_ELEM -4

typedef struct
{
    tNodo* pri;
    tNodo* ult;
}tCola;

void colaCrear(tCola* pc);
int colaEstaVacia(const tCola* pc);
int colaEncolar(tCola* pc, const void* info, unsigned tamInfo);
int colaDesencolar(tCola* pc, void* info, unsigned tamInfo);
void colaVaciar(tCola* pc);

#endif // ESTRUCTURAS_COLA_H_INCLUDED
