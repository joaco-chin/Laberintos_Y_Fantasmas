#include "interno_jugador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "principal_menu.h"
#include "interno_matriz.h"
#include "interno_laberinto.h"

int ingresarMovimiento() // Funcion hecha para solucionar el problema del doble movimiento
{
    int entrada = getch();
    if(entrada == 0 || entrada == 224)
    {
        entrada = getch();
    }
    return entrada;
}

char ingresarTeclaDeJugador(unsigned periodo)
{
    clock_t temp = clock() + periodo;
    char entrada = 0;

    fflush(stdin);
    while(temp > clock())
    {
        if(kbhit())
        {
            entrada = (char)ingresarMovimiento();
            return entrada;
        }
    }

    return entrada;
}

void matrizActualizarPosicionDeJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna)
{
    if (nuevaFila >= 0 && nuevaFila < filas && nuevaColumna >= 0 && nuevaColumna < col)
    {
        if (matriz[nuevaFila][nuevaColumna] != PARED)
        {
            matriz[jug->posFil][jug->posCol] = CAMINO;
            jug->posFil = nuevaFila;
            jug->posCol = nuevaColumna;

            actualizarPuntosYVidas(jug, matriz[jug->posFil][jug->posCol]);

            matriz[jug->posFil][jug->posCol] = JUGADOR;
        }
    }
}

void actualizarPuntosYVidas(tJugador *jug, char celda)
{
    if (celda == PREMIO)
        jug->puntos++;
    else if (celda == VIDA_EXTRA)
        jug->vidas++;
    else if (celda == FANTASMA)
        jug->vidas--;
}
