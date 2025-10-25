#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include "principal_archivo.h"

// Valores ASCII de las teclas
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESC 27
#define ENTER 13

#define JUGADOR 'J'

#define ES_MOVIMIENTO(x)((x) == ARRIBA || (x) == ABAJO || (x) == IZQUIERDA || (x) == DERECHA ? 1 : 0)

typedef struct
{
    int posFil;
    int posCol;
    int vidas;
    int puntos;
} tJugador;

int ingresarMovimiento();
char ingresarTeclaDeJugador(unsigned periodo);
void matrizActualizarPosicionDeJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna);
void actualizarPuntosYVidas(tJugador *jug, char celda);

#endif // JUGADOR_H_INCLUDED
