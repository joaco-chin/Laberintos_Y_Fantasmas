#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "codigosRet.h"
#include "juego.h"

// ignorar ***
#define FILAS 31
#define COLUMNAS 31
#define HAY_CARACTER -7
int hayCaracterAlrededor(char **matriz, int filas, int columnas, int i, int j, char objetivo);
void generarLaberintoAleatorio(char **matriz, int filas, int columnas, float densidad);
// ignorar ***

int main()
{
    juego();

    //    char **matriz = (char **)matrizCrear(sizeof(char), FILAS, COLUMNAS);
    //    generarLaberintoAleatorio(matriz, FILAS, COLUMNAS, 1);
    //    matrizMostrar(matriz, FILAS, COLUMNAS);
    //    matrizDestruir((void **)matriz, FILAS);

    return 0;
}

// ignorar despues crear laberinto.c y laberinto.h o mover en matriz.c y matriz.h
void generarLaberintoAleatorio(char **matriz, int filas, int columnas, float densidad)
{
    // inicializacion
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            matriz[i][j] = ' ';
        }
    }

    // paredes internas
    int numCantBloques;
    int sePudoHacerPared = 0;

    for (int i = 1; i < filas - 1; i++)
    {
        for (int j = 1; j < columnas - 1; j++)
        {

            if (hayCaracterAlrededor(matriz, FILAS, COLUMNAS, i, j, '#') != HAY_CARACTER)
            {

                // derecha
                numCantBloques = rand() % 4 + 1; // entre 1 y 4
                if (j + numCantBloques < columnas - 1)
                {
                    for (int k = 1; k <= numCantBloques; k++)
                        sePudoHacerPared = hayCaracterAlrededor(matriz, FILAS, COLUMNAS, i, j + k, '#');

                    if (sePudoHacerPared != HAY_CARACTER)
                        for (int k = 1; k <= numCantBloques; k++)
                            matriz[i][j + k] = '#';

                    sePudoHacerPared = 0;
                }

                // izquierda
                numCantBloques = rand() % 4 + 1; // entre 1 y 4
                if (j - numCantBloques > 0)
                {
                    for (int k = 1; k <= numCantBloques; k++)
                        sePudoHacerPared = hayCaracterAlrededor(matriz, FILAS, COLUMNAS, i, j - k, '#');

                    if (sePudoHacerPared != HAY_CARACTER)
                        for (int k = 1; k <= numCantBloques; k++)
                            matriz[i][j - k] = '#';

                    sePudoHacerPared = 0;
                }

                // arriba
                numCantBloques = rand() % 2 + 1; // entre 1 y 2
                if (i - numCantBloques > 0)
                {

                    for (int k = 1; k <= numCantBloques; k++)
                        sePudoHacerPared = hayCaracterAlrededor(matriz, FILAS, COLUMNAS, i - k, j, '#');

                    if (sePudoHacerPared != HAY_CARACTER)
                        for (int k = 1; k <= numCantBloques; k++)
                            matriz[i - k][j] = '#';

                    sePudoHacerPared = 0;
                }

                // abajo
                numCantBloques = rand() % 2 + 1; // entre 1 y 2
                if (i + numCantBloques < filas - 1)
                {
                    for (int k = 1; k <= numCantBloques; k++)
                        sePudoHacerPared = hayCaracterAlrededor(matriz, FILAS, COLUMNAS, i + k, j, '#');

                    if (sePudoHacerPared != HAY_CARACTER)
                        for (int k = 1; k <= numCantBloques; k++)
                            matriz[i + k][j] = '#';

                    sePudoHacerPared = 0;
                }

                matriz[i][j] = '#';
            }
        }
    }

    // paredes limite
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (i == 0 || i == filas - 1 || j == 0 || j == columnas - 1)
                matriz[i][j] = '#';
        }
    }

    // entrada y salida
    int numFil, numCol;
    srand(time(NULL));

    numFil = rand() % (filas); // entre 0 y filas
    printf("numFil: %d\n", numFil);

    if (numFil == 0 || numFil == filas - 1)
    {
        numCol = rand() % (columnas - 2) + 1;

        if (numFil == 0)
        {
            matriz[1][numCol] = '%';
            matriz[filas - numFil - 2][columnas - numCol - 1] = '-';
        }
        else if (numFil == filas - 1)
        {
            matriz[filas - 2][numCol] = '%';
            matriz[1][columnas - numCol - 1] = '-';
        }
    }
    else
    {
        numCol = (rand() % 2) ? 0 : columnas - 1; // si es 0 pone 0, si es 1 pone columnas-1

        if (numCol == 0)
        {
            matriz[numFil][1] = '%';
            matriz[filas - numFil - 1][columnas - 2] = '-';
        }
        else
        {
            matriz[numFil][columnas - 2] = '%';
            matriz[filas - numFil - 1][1] = '-';
        }
    }

    matriz[numFil][numCol] = 'E';
    matriz[filas - numFil - 1][columnas - numCol - 1] = 'S';
}

int hayCaracterAlrededor(char **matriz, int filas, int columnas, int i, int j, char objetivo)
{
    for (int k = i - 1; k <= i + 1; k++)
    {
        for (int l = j - 1; l <= j + 1; l++)
        {
            if (k >= 0 && k < filas && l >= 0 && l < columnas) // chequeo que no se salga de los limites
                if (matriz[k][l] == objetivo)
                    return HAY_CARACTER;
        }
    }
    return TODO_OK;
}
