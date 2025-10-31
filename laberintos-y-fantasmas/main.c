#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include "principal_menu.h"

#define TIEMPO_FRAME 500

int main()
{
    srand(time(NULL));
    initscr(); // Inicializa la pantalla de curses.h y asigna memoria
    keypad(stdscr, true); // Habilita el uso del keypad para la pantalla estándar
    noecho(); // Evita que el cursor titile
    curs_set(0); // Oculta el cursor

    timeout(TIEMPO_FRAME);
    menuPrincipal();

    refresh();
    endwin(); // Finaliza la pantalla de curses.h y libera memoria
    return 0;
}
