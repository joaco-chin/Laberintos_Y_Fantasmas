#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <winsock2.h>
#include "interno_pantalla.h"

#define CANT_OPCIONES_PAUSA 2
#define CANT_OPCIONES_CONFIRMACION 2

// Menu principal
#define NUEVA_PARTIDA 0
#define VER_RANKING 1
#define DIFICULTAD 2
#define SALIR 3
#define CANT_OPCIONES_PRINCIPAL 4

// Submenu de dificultad
#define OPCION_FACIL 0
#define OPCION_NORMAL 1
#define OPCION_DIFICIL 2
#define OPCION_PESADILLA 3
#define CANT_OPCIONES_DIFICULTAD 4

// Estado de Menu
#define REANUDAR 3
#define TERMINAR 0

void actualizarMenu(char matriz[][TAM_PAL_OPCION], int cf, int *posAct, int posNueva);
void mostrarMenu(char matriz[][TAM_PAL_OPCION], int filas);
void menuPrincipal(SOCKET sock, int altoStdscr, int anchoStdscr);
int menuDeConfirmacion(int altoStdscr, int anchoStdscr);
int menuDePausa(int altoStdscr, int anchoStdscr);
int subMenuDeDificultad(int altoStdscr, int anchoStdscr);
void menuDeDificultad(int altoStdscr, int anchoStdscr);

#endif // MENU_H_INCLUDED
