#include "interno_jugador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interno_matriz.h"
#include "principal_menu.h"

void matrizActualizarPosicionDeJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna)
{
    if (nuevaFila >= 0 && nuevaFila < filas && nuevaColumna >= 0 && nuevaColumna < col)
    {
        if (matriz[nuevaFila][nuevaColumna] != '#')
        {
            matriz[jug->posFil][jug->posCol] = ' ';
            jug->posFil = nuevaFila;
            jug->posCol = nuevaColumna;

            actualizarPuntosYVidas(jug, matriz[jug->posFil][jug->posCol]);

            matriz[jug->posFil][jug->posCol] = 'J';
        }
    }
}

void actualizarPuntosYVidas(tJugador *jug, char celda)
{
    if (celda == 'P')
        jug->puntos++;
    else if (celda == 'V')
        jug->vidas++;
    else if (celda == 'F')
        jug->vidas--;
}

int matrizActualizarPorEstadoDeVidas(char **matriz, tJugador *jug, tConfig *conf, int filaEntrada, int columnaEntrada)
{
    if (jug->vidas < conf->vidasInicio)
    {

        matriz[jug->posFil][jug->posCol] = ' ';
        jug->posFil = filaEntrada;
        jug->posCol = columnaEntrada;
        matriz[jug->posFil][jug->posCol] = 'J';
        conf->vidasInicio = jug->vidas;
    }
    else if (jug->vidas > conf->vidasInicio)
        conf->vidasInicio = jug->vidas;
    else if (jug->vidas <= 0)
    {
        printf("\n\nGame Over!\n");
        return TERMINAR;
    }

    return REANUDAR;
}
