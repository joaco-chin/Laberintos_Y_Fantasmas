#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define CANT_OPCIONES 3
#define CANT_OPCIONES_PAUSA 2
#define CANT_OPCIONES_CONFIRMACION 2
#define TAM_PAL 50

#define NUEVA_PARTIDA 0
#define VER_RANKING 1
#define SALIR 2

#define REANUDAR 3
#define TERMINAR 0

void limpiarConsola();
void actualizarMenu(char matriz[][TAM_PAL], int cf, int *posAct, int posNueva);
void mostrarMenu(char matriz[][TAM_PAL], int filas);
int menu();
int menuDePausa();
int menuDeConfirmacion();

#endif // MENU_H_INCLUDED
