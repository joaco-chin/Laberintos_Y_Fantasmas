#include "estructuras_pila.h"
#include "codigosRet.h"
#include <stdlib.h>
#include <string.h>
#include "generico.h"

void pilaCrear(tPila* pp)
{
    *pp = NULL;
}

int pilaEstaVacia(const tPila* pp)
{
    if(*pp == NULL)
        return PILA_VACIA;
    return PILA_CON_ELEM;
}

int pilaApilar(tPila* pp, const void* info, unsigned tamInfo)
{
    tNodo* nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo)
        return SIN_MEM;
    nuevo->info = malloc(tamInfo);
    if(!nuevo->info)
    {
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->info, info, tamInfo);
    nuevo->tamInfo = tamInfo;
    nuevo->sig = *pp;

    *pp =nuevo;

    return TODO_OK;
}

int pilaDesapilar(tPila* pp, void* info, unsigned tamInfo)
{
    tNodo* elim;

    if(*pp == NULL)
        return PILA_VACIA;

    elim = *pp;
    memcpy(info, elim->info, MIN(elim->tamInfo, tamInfo));
    *pp = elim->sig;

    free(elim->info);
    free(elim);
    return TODO_OK;
}

void pilaVaciar(tPila* pp)
{
    tNodo* elim;

    while(*pp)
    {
        elim = *pp;
        *pp = elim->sig;
        free(elim->info);
        free(elim);
    }
}
