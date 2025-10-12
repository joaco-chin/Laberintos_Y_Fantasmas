#include "interno_fantasma.h"
#include "codigosRet.h"
#include <math.h>

int aStarBusqueda(char** matriz, int cf, int cc, const tInfoNodo* inicio, const tInfoNodo* meta, tCola* movimientos)
{
    tLista openSet, closedSet;
    tCola vecinos;
    tPila camino;
    tInfoNodo nodoAct = {inicio->fil, inicio->col, 0, 0, 0, -1, -1}, nodoVecino, nodoAux;
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

        if(cmpPos(&nodoAct, meta) == 0) // Si llegamos a la meta, salimos
        {
//            puts("Camino encontrado");
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
                    if(nodoAct.fil == inicio->fil && nodoAct.col == inicio->col)
                    {
                        codigoDeError = pilaApilar(&camino, inicio, sizeof(tInfoNodo));
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

            while(codigoDeError == TODO_OK && pilaDesapilar(&camino, &nodoAct, sizeof(tInfoNodo)) == TODO_OK) // Reconstruimos el camino
            {
                codigoDeError = colaEncolar(movimientos, &nodoAct, sizeof(tInfoNodo));
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
                        if(listaBuscarPorClave(&closedSet, &nodoVecino, cmpPos) == -1 && matriz[nodoVecino.fil][nodoVecino.col] != '#')
                        {
                            // Actualizamos los valores
                            nodoVecino.costoG = nodoAct.costoG + 1;
                            nodoVecino.costoH = calcularHeuristica(&nodoVecino, meta);
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

    if(codigoDeError != TODO_OK)
    {
        listaVaciar(&openSet);
        listaVaciar(&closedSet);
        colaVaciar(&vecinos);
        pilaVaciar(&camino);
        return SIN_MEM;
    }

    if(metaAlcanzada != 1)
    {
//        puts("No existe un camino posible");
        listaVaciar(&openSet);
        listaVaciar(&closedSet);
        colaVaciar(&vecinos);
        pilaVaciar(&camino);
        return NO_EXISTE_CAMINO;
    }

    listaVaciar(&openSet);
    listaVaciar(&closedSet);
    colaVaciar(&vecinos);
    pilaVaciar(&camino);
    return TODO_OK;
}

//int calcularMovimientosFantasmas(char** matriz, int cf, int cc, tFantasma* fantasma, int ce, tJugador* jugador, tCola* movimientosTotales)
//{
//    tCola movimientos;
//    tFantasma* i = fantasma;
//    tFantasma* ult = fantasma + ce;
//
//    while(i < ult)
//    {
//
//
//        i++;
//    }
//
//    return TODO_OK;
//}

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
    const tInfoNodo* nodo1 = a;
    const tInfoNodo* nodo2 = b;
    return nodo1->costoF - nodo2->costoF;
}

int cmpPos(const void* a, const void* b)
{
    const tInfoNodo* nodo1 = a;
    const tInfoNodo* nodo2 = b;
    int resultado = nodo1->fil - nodo2->fil;

    if(resultado != 0)
        return resultado;

    return nodo1->col - nodo2->col;
}

