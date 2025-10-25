#include "interno_fantasma.h"
#include "principal_menu.h"
#include "interno_laberinto.h"
#include "codigosRet.h"
#include <math.h>
#include <stdio.h>

int aStarBusqueda(char** matriz, int cf, int cc, const tFantasma* origen, const tJugador* destino, tCola* movimientos)
{
    tLista openSet, closedSet;
    tCola vecinos;
    tPila camino;
    tInfoNodo nodoAct = {origen->fil, origen->col, 0, 0, 0, -1, -1, ' ', origen->posInicial}, nodoVecino, nodoAux;
    tInfoNodo inicio = {origen->fil, origen->col, 0, 0, 0, -1, -1, ' ', origen->posInicial}, meta = {destino->posFil, destino->posCol, 0, 0, 0, -1, -1};
    int pos;
    int metaAlcanzada = 0;
    int codigoDeError = TODO_OK;

    listaCrear(&openSet);
    listaCrear(&closedSet);
    colaCrear(&vecinos);
    pilaCrear(&camino);

    codigoDeError = listaInsertarAlInicio(&openSet, &nodoAct, sizeof(tInfoNodo)); // Insertamos el nodo actual que en el momento contiene al nodo inicial
    while(codigoDeError == TODO_OK && listaEstaVacia(&openSet) != LISTA_VACIA && metaAlcanzada != 1)
    {
        pos = listaBuscarMenor(&openSet, cmpCostos); // Buscamos el nodo con el menor costo F
        listaRemoverPorPos(&openSet, &nodoAct, sizeof(tInfoNodo), pos);

        if(cmpPos(&nodoAct, &meta) == 0) // Si llegamos a la meta, salimos
        {
            metaAlcanzada = 1;

            // Recorremos todos los nodos padres hasta volver al inicio
            codigoDeError = pilaApilar(&camino, &nodoAct, sizeof(tInfoNodo));
            while(codigoDeError == TODO_OK && nodoAct.filPadre != -1 && nodoAct.colPadre != -1)
            {
                nodoAct.fil = nodoAct.filPadre;
                nodoAct.col = nodoAct.colPadre;
                pos = listaBuscarPorClave(&closedSet, &nodoAct, cmpPos);

                if(pos == -1)
                {
                    if(nodoAct.fil == inicio.fil && nodoAct.col == inicio.col)
                    {
                        codigoDeError = pilaApilar(&camino, &inicio, sizeof(tInfoNodo));
                    }
                    else
                    {
                        codigoDeError = -7;
                    }
                }
                else
                {
                    listaRemoverPorPos(&closedSet, &nodoAct, sizeof(tInfoNodo), pos);
                    codigoDeError = pilaApilar(&camino, &nodoAct, sizeof(tInfoNodo));
                }
            }


            if(pilaDesapilar(&camino, &nodoAct, sizeof(tInfoNodo)) == TODO_OK)
            {
                if(pilaDesapilar(&camino, &nodoAct, sizeof(tInfoNodo)) == TODO_OK)
                {
                    nodoAct.caracterAnterior = origen->caracterAnterior;
                    nodoAct.posInicial = origen->posInicial;
                    codigoDeError = colaEncolar(movimientos, &nodoAct, sizeof(tInfoNodo));
                }
            }
        }
        else // Si no llegamos a la meta, seguimos
        {
            if((codigoDeError = listaInsertarAlInicio(&closedSet, &nodoAct, sizeof(tInfoNodo))) == TODO_OK)
            {
                if((codigoDeError = buscarVecinos(cf, cc, &nodoAct, &vecinos)) == TODO_OK)
                {
                    while(codigoDeError == TODO_OK && colaDesencolar(&vecinos, &nodoVecino, sizeof(tInfoNodo)) == TODO_OK)
                    {
                        if(listaBuscarPorClave(&closedSet, &nodoVecino, cmpPos) == -1 && matriz[nodoVecino.fil][nodoVecino.col] != PARED)
                        {
                            // Actualizamos los valores
                            nodoVecino.costoG = nodoAct.costoG + 1;
                            nodoVecino.costoH = calcularHeuristica(&nodoVecino, &meta);
                            nodoVecino.costoF = nodoVecino.costoG + nodoVecino.costoH;
                            nodoVecino.filPadre = nodoAct.fil;
                            nodoVecino.colPadre = nodoAct.col;

                            pos = listaBuscarPorClave(&openSet, &nodoVecino, cmpPos);
                            if(pos == -1)
                            {
                                codigoDeError = listaInsertarAlInicio(&openSet, &nodoVecino, sizeof(tInfoNodo));
                            }
                            else
                            {
                                listaRemoverPorPos(&openSet, &nodoAux, sizeof(tInfoNodo), pos);
                                if(nodoVecino.costoG < nodoAux.costoG)
                                {
                                    codigoDeError = listaInsertarAlInicio(&openSet, &nodoVecino, sizeof(tInfoNodo));
                                }
                                else
                                {
                                    codigoDeError = listaInsertarAlInicio(&openSet, &nodoAux, sizeof(tInfoNodo));
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    listaVaciar(&openSet);
    listaVaciar(&closedSet);
    colaVaciar(&vecinos);
    pilaVaciar(&camino);

    if(codigoDeError != TODO_OK)
        return codigoDeError;
    if(metaAlcanzada != 1)
        return NO_ENCONTRADO;
    return TODO_OK;
}

void calcularMovimientosFantasmas(char** matriz, int cf, int cc, tCola* colaFantasmas, const tJugador* jugador, tCola* movimientos)
{
    tFantasma fantasma;

    while(colaDesencolar(colaFantasmas, &fantasma, sizeof(tFantasma)) == TODO_OK)
    {
        aStarBusqueda(matriz, cf, cc, &fantasma, jugador, movimientos);
    }
}

void actualizarPosicionesFantasmas(char** matriz, int cf, int cc, tCola* colaFantasmas, tJugador* jugador, tCola* movimientos)
{
    tFantasma fantasma;
    tInfoNodo movFantasma;

    while(colaDesencolar(movimientos, &movFantasma, sizeof(tInfoNodo)) == TODO_OK)
    {
//        printf("fantasma|fil:%d|col:%d\n", movFantasma.fil, movFantasma.col); // TEST
        matrizRemplazarCaracterEnPosicion(matriz, movFantasma.caracterAnterior, movFantasma.filPadre, movFantasma.colPadre, cf, cc);

        fantasma.fil = movFantasma.fil;
        fantasma.col = movFantasma.col;
        fantasma.posInicial = movFantasma.posInicial;

        if(matriz[movFantasma.fil][movFantasma.col] != FANTASMA && matriz[movFantasma.fil][movFantasma.col] != JUGADOR)
            fantasma.caracterAnterior = matriz[movFantasma.fil][movFantasma.col];

        matrizRemplazarCaracterEnPosicion(matriz, FANTASMA, fantasma.fil, fantasma.col, cf, cc);
        colaEncolar(colaFantasmas, &fantasma, sizeof(tFantasma));
    }
}

int calcularHeuristica(const tInfoNodo* inicio, const tInfoNodo* meta)
{
    int x = abs(meta->col - inicio->col);
    int y = abs(meta->fil - inicio->fil);
    return x + y;
}

int buscarVecinos(int cf, int cc, const tInfoNodo* nodoAct, tCola* plVecinos) // Funciona para todos los casos de prueba
{
    tInfoNodo nodoVecino;
    int limI = MIN(cf, nodoAct->fil + 2);
    int limJ = MIN(cc, nodoAct->col + 2);
    int retorno = TODO_OK;

    nodoVecino.fil = MAX(0, nodoAct->fil - 1); // nodoVecino.fil = i
    while(retorno == TODO_OK && nodoVecino.fil < limI)
    {
        nodoVecino.col = MAX(0, nodoAct->col - 1); // nodoVecino.col = j
        while(retorno == TODO_OK && nodoVecino.col < limJ)
        {
            if((nodoVecino.fil == nodoAct->fil || nodoVecino.col == nodoAct->col) && !(nodoVecino.fil == nodoAct->fil && nodoVecino.col == nodoAct->col))
            {
                nodoVecino.costoG = 0;
                nodoVecino.costoH = 0;
                nodoVecino.costoF = 0;
                retorno = colaEncolar(plVecinos, &nodoVecino, sizeof(tInfoNodo));
            }
            nodoVecino.col++;
        }
        nodoVecino.fil++;
    }

    if(retorno != TODO_OK)
        return retorno;

    return TODO_OK;
}

int cmpCostos(const void* a, const void* b)
{
    const tInfoNodo* nodo_1 = a;
    const tInfoNodo* nodo_2 = b;
    return nodo_1->costoF - nodo_2->costoF;
}

int cmpPos(const void* a, const void* b)
{
    const tInfoNodo* nodo_1 = a;
    const tInfoNodo* nodo_2 = b;
    int resultado = nodo_1->fil - nodo_2->fil;

    if(resultado != 0)
        return resultado;

    return nodo_1->col - nodo_2->col;
}

int cmpPosFan(const void* a, const void* b)
{
    const tFantasma* fantasma_1 = a;
    const tFantasma* fantasma_2 = b;
    int resultado = fantasma_1->fil - fantasma_2->fil;

    if(resultado != 0)
        return resultado;

    return fantasma_1->col - fantasma_2->col;
}


