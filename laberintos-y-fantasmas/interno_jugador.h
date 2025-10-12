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

typedef struct
{
    int posFil;
    int posCol;
    /*
    int posFilEnt;  Para hacer mas eficiente el codigo
    int posColEnt;  No fueron reemplazadas todavia
    int posFilSal;
    int posColSal;
    */
    int vidas;
    int puntos;
} tJugador;

void matrizActualizarPosicionDeJugador(char **matriz, int filas, int col, tJugador *jug, int nuevaFila, int nuevaColumna);
void actualizarPuntosYVidas(tJugador *jug, char celda);
void matrizActualizarPorEstadoDeVidas(char **matriz, tJugador *jug, tConfig *conf, int filaEntrada, int columnaEntrada, char *tecla);

#endif // JUGADOR_H_INCLUDED
