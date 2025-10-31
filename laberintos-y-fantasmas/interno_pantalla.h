#ifndef INTERNO_PANTALLA_H_INCLUDED
#define INTERNO_PANTALLA_H_INCLUDED

#include <curses.h>

#define TAM_PAL_OPCION 50
#define SIN_TITULO "\0"
#define TIEMPO_MENSAJE 1000

void dibujarTitulos(int altoStdscr, int anchoStdscr);
void dibujarMenu(char matriz[][TAM_PAL_OPCION], int cf, const char* titulo, int altoStdscr, int anchoStdscr);
void dibujarInicioPartida(char **matriz, int cc, int cf);
void dibujarPartida(char **matriz, int cc, int cf, const char *dificultad, int vidas, int puntos);
void dibujarMensaje(const char* mensaje);

#endif // INTERNO_PANTALLA_H_INCLUDED
