#include "principal_menu.h"
#include <stdio.h>
#include <windows.h> // para limpiar consola
#include "codigosRet.h"
#include "principal_partida.h"
#include "interno_cliente.h"
#include "interno_jugador.h"

void menuPrincipal(SOCKET sock, int altoStdscr, int anchoStdscr)
{
    int pos = 0;
    int tecla;
    char matPantalla[CANT_OPCIONES_PRINCIPAL][TAM_PAL_OPCION] =
    {
        "-> Nueva Partida",
        "   Ver Ranking",
        "   Dificultad",
        "   Salir",
    };

    dibujarTitulos(altoStdscr, anchoStdscr);

    tecla = getch();
    while(tecla != ENTER)
    {
        tecla = getch();
    }

    dibujarMenu(matPantalla, CANT_OPCIONES_PRINCIPAL, SIN_TITULO, altoStdscr, anchoStdscr);

    while (!(pos == SALIR && tecla == ENTER))
    {
        tecla = getch();

        if(tecla == KEY_DOWN || tecla == KEY_UP)
        {
            actualizarMenu(matPantalla, CANT_OPCIONES_PRINCIPAL, &pos, pos + (tecla == KEY_DOWN) - (tecla == KEY_UP));
        }

        if (pos != SALIR && tecla == ENTER)
        {
            if (pos == 0)
            {
                configuracionPartida(sock, altoStdscr, anchoStdscr);
            }

            else if(pos == 1)
            {
                verRanking(sock);
            }

            else if(pos == 2)
            {
                menuDeDificultad(altoStdscr, anchoStdscr);
            }
        }

        dibujarMenu(matPantalla, CANT_OPCIONES_PRINCIPAL, SIN_TITULO, altoStdscr, anchoStdscr);
    }
}

int menuDeConfirmacion(int altoStdscr, int anchoStdscr)
{
    int pos = 0;
    int tecla;
    char matrizPantalla[CANT_OPCIONES_CONFIRMACION][TAM_PAL_OPCION] =
    {
        "-> No",
        "   Si"
    };

    dibujarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION, "Seguro que desea salir? Su partida se tomara como perdida\n", altoStdscr, anchoStdscr);
    tecla = getch();

    while(tecla != ESC && tecla != ENTER)
    {
        if(tecla == KEY_DOWN || tecla == KEY_UP)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION, &pos, pos + (tecla == KEY_DOWN) - (tecla == KEY_UP));
        }

        dibujarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION, "Seguro que desea salir? Su partida se tomara como perdida\n", altoStdscr, anchoStdscr);
        tecla = getch();
    }

    if(tecla == ESC || (pos == 0 && tecla == ENTER))
    {
        return REANUDAR;
    }

    return TERMINAR;
}

int menuDePausa(int altoStdscr, int anchoStdscr)
{
    int pos = 0;
    int tecla;
    char matrizPantalla[CANT_OPCIONES_PAUSA][TAM_PAL_OPCION] =
    {
        "-> Volver al juego",
        "   Salir al menu principal"
    };
    int salida = REANUDAR;

    dibujarMenu(matrizPantalla, CANT_OPCIONES_PAUSA, "- PAUSA -", altoStdscr, anchoStdscr);
    tecla = getch();
    while(salida == REANUDAR && !(tecla == ESC || (pos == 0 && tecla == ENTER)))
    {
        if(pos == 1 && tecla == ENTER)
        {
            salida = menuDeConfirmacion(altoStdscr, anchoStdscr);
        }

        else if(tecla == KEY_DOWN || tecla == KEY_UP)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_PAUSA, &pos, pos + (tecla == KEY_DOWN) - (tecla == KEY_UP));
        }

        if(salida == REANUDAR)
        {
            dibujarMenu(matrizPantalla, CANT_OPCIONES_PAUSA, "- PAUSA -", altoStdscr, anchoStdscr);
            tecla = getch();
        }
    }

    return salida;
}

int subMenuDeDificultad(int altoStdscr, int anchoStdscr)
{
    int pos = OPCION_NORMAL;
    int tecla;
    char matrizPantalla[CANT_OPCIONES_DIFICULTAD][TAM_PAL_OPCION] =
    {
        "   Facil",
        "-> Normal",
        "   Dificil",
        "   Pesadilla"
    };
    FILE* pf = fopen("config.txt", "wt");
    if(!pf)
    {
        return ERR_ARCHIVO;
    }

    dibujarMenu(matrizPantalla, CANT_OPCIONES_DIFICULTAD, "- DIFICULTAD -", altoStdscr, anchoStdscr);
    tecla = getch();

    while(tecla != ENTER)
    {
        if(tecla == KEY_DOWN || tecla == KEY_UP)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_DIFICULTAD, &pos, pos + (tecla == KEY_DOWN) - (tecla == KEY_UP));
        }

        dibujarMenu(matrizPantalla, CANT_OPCIONES_DIFICULTAD, "- DIFICULTAD -", altoStdscr, anchoStdscr);
        tecla = getch();
    }

    switch(pos)
    {
    case OPCION_FACIL:
        fprintf(pf, "%d|%d|%d|%d|%d|%d|FACIL\n", 27, 55, 4, 4, 10, 4);
        break;
    case OPCION_NORMAL:
        fprintf(pf, "%d|%d|%d|%d|%d|%d|NORMAL\n", 27, 55, 3, 5, 10, 2);
        break;
    case OPCION_DIFICIL:
        fprintf(pf, "%d|%d|%d|%d|%d|%d|DIFICIL\n", 27, 55, 3, 6, 10, 2);
        break;
    case OPCION_PESADILLA:
        fprintf(pf, "%d|%d|%d|%d|%d|%d|PESADILLA\n", 27, 55, 1, 6, 10, 0);
        break;
    }

    fclose(pf);
    return TODO_OK;
}

void menuDeDificultad(int altoStdscr, int anchoStdscr)
{
    int tecla;
    int pos = 0;
    char matrizPantalla[CANT_OPCIONES_PAUSA][TAM_PAL_OPCION] =
    {
        "-> Seleccionar dificultad",
        "   Volver al menu principal"
    };

    dibujarMenu(matrizPantalla, CANT_OPCIONES_PAUSA, "- DIFICULTAD -", altoStdscr, anchoStdscr);
    tecla = getch();

    while(!(pos == 1 && tecla == ENTER))
    {
        if(pos == 0 && tecla == ENTER)
        {
            if(subMenuDeDificultad(altoStdscr, anchoStdscr) == ERR_ARCHIVO)
            {
                fprintf(stderr, "Error en la apertura del archivo\n");
            }
        }

        if(tecla == KEY_UP || tecla == KEY_DOWN)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION, &pos, pos + (tecla == KEY_DOWN) - (tecla == KEY_UP));
        }

        dibujarMenu(matrizPantalla, CANT_OPCIONES_PAUSA, "- DIFICULTAD -", altoStdscr, anchoStdscr);
        tecla = getch();
    }
}

void actualizarMenu(char matriz[][TAM_PAL_OPCION], int cf, int *posAct, int posNueva)
{
    int j = 0;

    if (posNueva >= 0 && posNueva < cf)
    {
        while (matriz[*posAct][j] != ' ')
        {
            matriz[*posAct][j] = ' ';
            j++;
        }

        *posAct = posNueva;

        matriz[*posAct][0] = '-';
        matriz[*posAct][1] = '>';
    }
}

void mostrarMenu(char matriz[][TAM_PAL_OPCION], int filas)
{
    int i;

    for (i = 0; i < filas; i++)
    {
        puts(matriz[i]);
        putchar('\n');
    }
}

