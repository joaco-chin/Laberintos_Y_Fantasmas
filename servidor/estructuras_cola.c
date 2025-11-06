#include "estructuras_cola.h"
#include "codigosRet.h"
#include <stdlib.h>
#include <string.h>
#include "generico.h"

void colaCrear(tCola* pc)
{
    pc->pri = NULL;
    pc->ult = NULL;
}

int colaEstaVacia(const tCola* pc)
{
    if(pc->pri == NULL)
        return COLA_VACIA;
    return COLA_CON_ELEM;
}

int colaEncolar(tCola* pc, const void* info, unsigned tamInfo)
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
    nuevo->sig = NULL;

    if(pc->pri == NULL)
        pc->pri = nuevo;
    else
        pc->ult->sig = nuevo;

    pc->ult = nuevo;

    return TODO_OK;
}

int colaDesencolar(tCola* pc, void* info, unsigned tamInfo)
{
    tNodo* elim;

    if(pc->pri == NULL)
        return COLA_VACIA;

    elim = pc->pri;
    memcpy(info, elim->info, MIN(elim->tamInfo, tamInfo));
    pc->pri = elim->sig;

    free(elim->info);
    free(elim);

    if(pc->pri == NULL)
        pc->ult = NULL;

    return TODO_OK;
}

void colaVaciar(tCola* pc)
{
    tNodo* elim;

    while(pc->pri)
    {
        elim = pc->pri;
        pc->pri = elim->sig;
        free(elim->info);
        free(elim);
    }

    pc->ult = NULL;
}
