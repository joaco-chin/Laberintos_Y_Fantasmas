#include "interno_jugador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "principal_menu.h"
#include "interno_matriz.h"

void moverJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna, tCola* colaMovimientos)
{
    int codigoDeError = TODO_OK;
    tEntidad mov;

    if(nuevaFila >= 0 && nuevaFila < filas && nuevaColumna >= 0 && nuevaColumna < col) // Mientras esté dentro de la matriz
    {
        if(matriz[nuevaFila][nuevaColumna] != PARED)
        {
            mov = jug->inGame;
            mov.y = nuevaFila;
            mov.x = nuevaColumna;
            mov.caracterFondo = matriz[nuevaFila][nuevaColumna];
            matriz[jug->inGame.y][jug->inGame.x] = jug->inGame.caracterFondo;
            codigoDeError = colaEncolar(colaMovimientos, &mov, sizeof(tEntidad));
            if(codigoDeError == TODO_OK)
            {
                jug->inGame.y = nuevaFila;
                jug->inGame.x = nuevaColumna;
                actualizarPuntosYVidas(jug, matriz);
            }
        }
    }
}

void actualizarPuntosYVidas(tJugador *jug, char **matriz)
{
    if(matriz[jug->inGame.y][jug->inGame.x] == PREMIO)
    {
        jug->puntos++;
        matriz[jug->inGame.y][jug->inGame.x] = CAMINO;
    }
    else if(matriz[jug->inGame.y][jug->inGame.x] == VIDA_EXTRA)
    {
        jug->vidas++;
        matriz[jug->inGame.y][jug->inGame.x] = CAMINO;
    }
    else if(matriz[jug->inGame.y][jug->inGame.x] == FANTASMA)
    {
        jug->vidas--;
        matriz[jug->inGame.y][jug->inGame.x] = CAMINO;
    }
}

void impPosEnPantalla(const void* a)
{
    const tPosicion* posJugador = (tPosicion*)a;
    printw("Fila:%d|Columna%d\n", posJugador->fila, posJugador->columna);
    refresh();
    napms(TIEMPO_MENSAJE_MOV);
}
