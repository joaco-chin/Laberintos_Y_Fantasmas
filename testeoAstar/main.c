#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include "estructuras_lista.h"
#include "estructuras_cola.h"
#include "estructuras_pila.h"

#define COL 19
#define TODO_OK 0
#define SIN_MEM -1
#define NO_ENCONTRADO -2
#define COSTO_MAX 100000000

typedef struct
{
    int fil;
    int col;
    int costoG;
    int costoH;
    int costoF;
    int filPadre;
    int colPadre;
}tInfoNodo;

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

void impNodo_TEST(const void* a) // Funcion de testing
{
    const tInfoNodo* n = a;
    printf("Fila: %d|Col: %d|Costo G: %d|Costo H: %d|Costo F: %d\n", n->fil + 1, n->col + 1, n->costoG, n->costoH, n->costoF);
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

int calcularHeuristica(const tInfoNodo* inicio, const tInfoNodo* meta)
{
    int x = abs(meta->col - inicio->col);
    int y = abs(meta->fil - inicio->fil);
    return x + y;
}

int aStarBusqueda(char matriz[][COL], int cf, int cc, const tInfoNodo* inicio, const tInfoNodo* meta, tCola* movimientos)
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
            puts("Camino encontrado");
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
        puts("No existe un camino posible");
        listaVaciar(&openSet);
        listaVaciar(&closedSet);
        colaVaciar(&vecinos);
        pilaVaciar(&camino);
        return NO_ENCONTRADO;
    }

    listaVaciar(&openSet);
    listaVaciar(&closedSet);
    colaVaciar(&vecinos);
    pilaVaciar(&camino);
    return TODO_OK;
}

void matrizRemplazarCaracterEnPosicion(char matriz[][COL], char nuevoCaracter, int fila, int columna, int cf, int cc)
{
    if (fila >= 0 && fila < cf && columna >= 0 && columna < cc)
        matriz[fila][columna] = nuevoCaracter;
}

void matrizMostrar(char matriz[][COL], int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
            printf("%c ", matriz[i][j]);
        printf("\n");
    }
}

char matrizBuscar(char matriz[][COL], char objetivo, int *fila, int *columna, int cf, int cc)
{
    for (int i = 0; i < cf; i++)
    {
        for (int j = 0; j < cc; j++)
        {
            if (matriz[i][j] == objetivo)
            {
                *fila = i;
                *columna = j;
                return matriz[i][j];
            }
        }
    }
    *fila = -1;
    *columna = -1;
    return '\0';
}

void limpiarConsola() // lo mismo que poner system("cls");
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(
            hConsole,
            (TCHAR)' ',
            cellCount,
            homeCoords,
            &count))
        return;

    SetConsoleCursorPosition(hConsole, homeCoords);
}

int main()
{
    char matriz[21][COL] =
    {
        "###################",
        "#   J    #        #",
        "# ## ### # ### ## #",
        "#                 #",
        "# ## # ##### # ## #",
        "#    #   #   #    #",
        "#### ### # ### ####",
        "#                 #",
        "### # ##   ## # ###",
        "#     #     #     #",
        "### P ##   ##   ###",
        "#  # #       # #  #",
        "# ## #  ###  # ## #",
        "#      P #   V    #",
        "# ## ### # ### ## #",
        "#  #           #  #",
        "## # # ##### # # ##",
        "#    #   #   #    #",
        "# ###### # ###### #",
        "#                F#",
        "###################"
    };
    tCola movimientos;
//    tInfoNodo f = {19, 17, 0, 0, 0}, j = {1, 11, 0, 0, 0}, act;
    tInfoNodo f, j, act;
    char objeto = ' ';

    matrizBuscar(matriz, 'F', &f.fil, &f.col, 21, COL);
    matrizBuscar(matriz, 'J', &j.fil, &j.col, 21, COL);
    f.costoG = 0;
    f.costoH = 0;
    f.costoF = 0;
    f.filPadre = -1;
    f.colPadre = -1;
    j.costoG = 0;
    j.costoH = 0;
    j.costoF = 0;
    j.filPadre = -1;
    j.colPadre = -1;
    colaCrear(&movimientos);

    if(aStarBusqueda(matriz, 21, COL, &f, &j, &movimientos) == TODO_OK)
    {
        // Pintar camino
//        while(colaDesencolar(&movimientos, &act, sizeof(tInfoNodo)) == TODO_OK)
//        {
//            limpiarConsola();
//            matrizRemplazarCaracterEnPosicion(matriz, 'C', act.fil, act.col, 21, COL);
//            matrizMostrar(matriz, 21, COL);
//            Sleep(10);
//        }

        // Mover fantasma
        while(colaDesencolar(&movimientos, &act, sizeof(tInfoNodo)) == TODO_OK)
        {
            limpiarConsola();
            matrizRemplazarCaracterEnPosicion(matriz, objeto, f.fil, f.col, 21, COL);
            objeto = matriz[act.fil][act.col];
            matrizRemplazarCaracterEnPosicion(matriz, 'F', act.fil, act.col, 21, COL);

            matrizMostrar(matriz, 21, COL);
            Sleep(150);
            f = act;
        }
    }

    colaVaciar(&movimientos);
    return 0;
}
