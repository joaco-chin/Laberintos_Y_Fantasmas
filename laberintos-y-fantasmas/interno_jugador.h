#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include "principal_archivo.h"
#include "interno_laberinto.h"
//#include <curses.h>

// Valores ASCII de las teclas
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESC 27
#define ENTER 10

#define JUGADOR 'J'

typedef struct
{
    tEntidad inGame;
    int vidas;
    int puntos;
    int cantMovimientos;
}tJugador;

//void matrizActualizarPosicionDeJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna);
//void actualizarPuntosYVidas(tJugador *jug, char celda);
void moverJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna, tCola* colaMovimientos);
void actualizarPuntosYVidas(tJugador *jug, char **matriz);
void impPosEnPantalla(const void* a);

#endif // JUGADOR_H_INCLUDED
