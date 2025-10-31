#include "interno_pantalla.h"
#include <string.h>

void dibujarTitulos(int altoStdscr, int anchoStdscr)
{
    int tamTitulo = strlen("- LABERINTOS Y FANTASMAS -\n");
    int tamTexto = strlen("- Presione Enter -\n");
    clear();
    mvprintw(altoStdscr / 2, (anchoStdscr - tamTitulo) / 2, "- LABERINTOS Y FANTASMAS -\n");
    mvprintw((altoStdscr / 2) + 1, ((anchoStdscr - tamTexto) / 2) - 1,"- Presione Enter -\n");
    refresh();
}

void dibujarMenu(char matriz[][TAM_PAL_OPCION], int cf, const char* titulo, int altoStdscr, int anchoStdscr)
{
    int i;

    clear();

    if(titulo)
    {
//        mvprintw(altoStdscr / 2, (anchoStdscr - strlen(titulo)) / 2, "%s\n", titulo);
        printw("%s\n", titulo);
    }
    for(i = 0; i < cf; i++)
    {
//        mvprintw((altoStdscr / 2) + i + 1, (anchoStdscr - strlen(matriz[i])) / 2, "%s\n", matriz[i]);
        printw("%s\n", matriz[i]);
    }

    refresh();
}

void dibujarInicioPartida(char **matriz, int cf, int cc)
{
    int i, j;
    clear();
    mvprintw(0, 0, "Presione una tecla para comenzar\n");
    for(i = 0; i < cf; i++)
    {
        for(j = 0; j < cc; j++)
        {
            mvaddch(i + 3, j * 2, matriz[i][j]);
        }
    }

    refresh();
}

void dibujarPartida(char **matriz, int cf, int cc, const char *dificultad, int vidas, int puntos)
{
    int i, j;

    clear();
    mvprintw(0, 0, "Vidas: %d\nPuntos: %d\nDificultad: %s\n", vidas, puntos, dificultad);

    for(i = 0; i < cf; i++)
    {
        for(j = 0; j < cc; j++)
        {
            mvaddch(i + 3, j * 2, matriz[i][j]);
        }
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
