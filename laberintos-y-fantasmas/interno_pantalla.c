#include "interno_pantalla.h"
#include <string.h>

void dibujarTitulos()
{
    int maximoY, maximoX;
    int tamTitulo = strlen("- LABERINTOS Y FANTASMAS -\n");
    int tamTexto = strlen("- Presione Enter -\n");
    getmaxyx(stdscr, maximoY, maximoX);
    clear();
    mvprintw(maximoY / 2, (maximoX - tamTitulo) / 2, "- LABERINTOS Y FANTASMAS -\n");
    mvprintw((maximoY / 2) + 1, ((maximoX - tamTexto) / 2) - 1,"- Presione Enter -\n");
    refresh();
}

void dibujarMenu(char matriz[][TAM_PAL_OPCION], int filas, const char* titulo)
{
    int i;

    clear();

    if(titulo)
    {
        printw("%s\n", titulo);
    }
    for (i = 0; i < filas; i++)
    {
        printw("%s\n", matriz[i]);
    }

    refresh();
}

void dibujarInicioPartida(char **matriz, int cf, int cc)
{
    int i, j;
    clear();
    mvprintw(0, 0, "Presione una tecla para comenzar, finaliza con ESC\n\n");
    for(i = 0; i < cf; i++)
    {
        for(j = 0; j < cc; j++)
        {
//            printw("%c ", matriz[i][j]);
            mvaddch(i + 2, j * 2, matriz[i][j]);
        }
            // matriz[i][j] == ' ' ? printf("- ") : printf("%c ", matriz[i][j]);
//        printw("\n");
    }

    refresh();
//    Sleep(TIEMPO_MENSAJE);
}

void dibujarPartida(char **matriz, int cf, int cc, const char *dificultad, int vidas, int puntos)
{
    int i, j;

    clear();
    //    printf("Dificultad: %s\n", dificultad);
    mvprintw(0, 0, "Vidas: %d\nPuntos: %d\n", vidas, puntos);

    for(i = 0; i < cf; i++)
    {
        for(j = 0; j < cc; j++)
        {
//            printw("%c ", matriz[i][j]);
            mvaddch(i + 2, j * 2, matriz[i][j]);
        }
            // matriz[i][j] == ' ' ? printf("- ") : printf("%c ", matriz[i][j]);
//        printw("\n");
    }

    refresh();
}

void dibujarMensaje(const char* mensaje)
{
    clear();
    mvprintw(0, 0, "%s\n", mensaje);
    refresh();
    napms(TIEMPO_MENSAJE);
}
