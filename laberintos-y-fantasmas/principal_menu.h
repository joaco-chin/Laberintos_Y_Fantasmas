#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define CANT_OPCIONES 3
#define TAM_PAL 50

#define NUEVA_PARTIDA 0
#define VER_RANKING 1
#define SALIR 2

void limpiarConsola();
void actualizarMenu(char matriz[CANT_OPCIONES][TAM_PAL], int *posAct, int posNueva);
void mostrarMenu(char matriz[CANT_OPCIONES][TAM_PAL], int filas);
int menu();

#endif // MENU_H_INCLUDED
