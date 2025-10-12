#include "interno_matriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigosRet.h"

void **matrizCrear(unsigned tamElem, int filas, int columnas)
{
    void **matriz = malloc(filas * sizeof(void *));

    if (matriz == NULL)
    {
        return NULL;
    }

    void **i;
    void **ult = matriz + filas - 1;

    for (i = matriz; i <= ult; i += tamElem)
    {
        *i = malloc(tamElem * columnas);

        if (matriz == NULL)
        {
            matrizDestruir(matriz, i - matriz);
            return NULL;
        }
    }

    return matriz;
}

void matrizMostrar(char **matriz, int filas, int columnas)
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
            printf("%c ", matriz[i][j]);
            // matriz[i][j] == ' ' ? printf("- ") : printf("%c ", matriz[i][j]);
        printf("\n");
    }
}

void matrizBuscar(char **matriz, char objetivo, int *fila, int *columna, int cf, int cc)
{
    for (int i = 0; i < cf; i++)
    {
        for (int j = 0; j < cc; j++)
        {
            if (matriz[i][j] == objetivo)
            {
                *fila = i;
                *columna = j;
                return;
            }
        }
    }
    *fila = -1;
    *columna = -1;
}

void matrizRemplazarCaracterEnPosicion(char **matriz, char nuevoCaracter, int fila, int columna, int cf, int cc)
{
    if (fila >= 0 && fila < cf && columna >= 0 && columna < cc)
        matriz[fila][columna] = nuevoCaracter;
}

void matrizDestruir(void **matriz, int filas)
{
    void **i;
    void **ult = matriz + filas - 1;

    for (i = matriz; i <= ult; i++)
    {
        free(*i);
    }

    free(matriz);
}
