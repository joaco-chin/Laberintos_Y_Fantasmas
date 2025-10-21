#include "estructuras_lista.h"
#include <stdlib.h>
#include <string.h>

void listaCrear(tLista* pl)
{
    *pl = NULL;
}

int listaEstaVacia(const tLista* pl)
{
    if(*pl == NULL)
        return LISTA_VACIA;
    return LISTA_CON_ELEM;
}

int listaInsertarAlInicio(tLista* pl, const void* info, unsigned tamInfo)
{
    tNodo* nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo)
        return -1;
    nuevo->info = malloc(tamInfo);
    if(!nuevo->info)
    {
        free(nuevo);
        return -1;
    }

    memcpy(nuevo->info, info, tamInfo);
    nuevo->tamInfo = tamInfo;

    nuevo->sig = *pl;
    *pl = nuevo;
    return 0;
}

int listaBuscarMenor(const tLista* pl, Cmp cmpNodos)
{
    int pos = 0;
    int min = pos;
    tNodo* menor = *pl;

    if(*pl == NULL)
        return LISTA_VACIA;

    while(*pl)
    {
        if(cmpNodos(menor->info, (*pl)->info) > 0)
        {
            menor = *pl;
            min = pos;
        }

        pos++;
        pl = &(*pl)->sig;
    }

    return min;
}

int listaBuscarPorClave(const tLista* pl, void* clave, Cmp cmpNodos)
{
    int pos = 0;
    int cmp = 1;

    while(*pl && (cmp = cmpNodos((*pl)->info, clave)) != 0)
    {
        pos++;
        pl = &(*pl)->sig;
    }

    if(cmp != 0)
        return -1;
    return pos;
}

int listaInsertarOrdenado(tLista* pl, const void* info, unsigned tamInfo, Cmp cmpNodos)
{
    tNodo* nuevo;
    int compInfoElem = 1;

    while(*pl && (compInfoElem = cmpNodos(info, (*pl)->info)) > 0)
    {
        pl = &(*pl)->sig;
    }

    nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo)
        return -1;

    nuevo->info = malloc(tamInfo);

    if(!nuevo->info)
    {
        free(nuevo);
        return -1;
    }

    memcpy(nuevo->info, info, tamInfo);
    nuevo->tamInfo = tamInfo;

    nuevo->sig = *pl;
    *pl = nuevo;

    return 0;
}

int listaRemoverPorPos(tLista* pl, void* info, unsigned tamInfo, int pos)
{
    tNodo* elim;
    tNodo* ant = NULL;
    int posAct = 0;

    if(*pl == NULL)
        return LISTA_VACIA;

    while(*pl && posAct < pos)
    {
        ant = *pl;
        posAct++;
        pl = &(*pl)->sig;
    }

    if(posAct != pos)
        return -1;

    elim = *pl;
    memcpy(info, elim->info, MIN(elim->tamInfo, tamInfo));
    *pl = elim->sig;
    if(ant)
        ant->sig = *pl;

    free(elim->info);
    free(elim);
    return 0;
}

int listaRemoverInicio(tLista* pl, void* info, unsigned tamInfo)
{
    tNodo* elim;

    if(*pl == NULL)
        return LISTA_VACIA;

    elim = *pl;
    memcpy(info, elim->info, MIN(elim->tamInfo, tamInfo));

    *pl = elim->sig;
    free(elim->info);
    free(elim);
    return 0;
}

int listaVerPos(const tLista* pl, void* info, unsigned tamInfo, int pos)
{
    int posAct = 0;

    while(*pl && posAct < pos)
    {
        pl = &(*pl)->sig;
    }

    if(posAct != pos)
        return -1;

    memcpy(info, (*pl)->info, MIN((*pl)->tamInfo, tamInfo));
    return 0;
}

void listaRecorrer_TEST(const tLista* pl, Accion imp)
{
    while(*pl)
    {
        imp((*pl)->info);
        pl = &(*pl)->sig;
    }
}

void listaVaciar(tLista* pl)
{
    tNodo* elim;

    while(*pl)
    {
        elim = *pl;
        pl = &(*pl)->sig;
        free(elim->info);
        free(elim);
    }
}


