#ifndef ESTRUCTURAS_PILA_H_INCLUDED
#define ESTRUCTURAS_PILA_H_INCLUDED

#include "estructuras_comun.h"
#define PILA_VACIA -3
#define PILA_CON_ELEM -4

typedef tNodo* tPila;

void pilaCrear(tPila* pp);
int pilaEstaVacia(const tPila* pp);
int pilaApilar(tPila* pp, const void* info, unsigned tamInfo);
int pilaDesapilar(tPila* pp, void* info, unsigned tamInfo);
void pilaVaciar(tPila* pp);

#endif // ESTRUCTURAS_PILA_H_INCLUDED
