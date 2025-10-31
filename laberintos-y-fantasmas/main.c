#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <winsock2.h>
#include "principal_menu.h"
#include "interno_cliente.h"

#define TIEMPO_FRAME 500

int main()
{
    SOCKET sock;
    int altoStdscr, anchoStdscr;

    srand(time(NULL));
    initscr(); // Inicializa la pantalla de curses.h y asigna memoria
    keypad(stdscr, true); // Habilita el uso del keypad para la pantalla estándar
    curs_set(0); // Oculta el cursor
    getmaxyx(stdscr, altoStdscr, anchoStdscr);

    sock = clienteConectarAlServidor();

//    nodelay(stdscr, true);
    noecho(); //

    timeout(TIEMPO_FRAME);
    menuPrincipal(sock, altoStdscr, anchoStdscr);

    refresh();
    endwin(); // Finaliza la pantalla de curses.h y libera memoria
    return 0;
}
