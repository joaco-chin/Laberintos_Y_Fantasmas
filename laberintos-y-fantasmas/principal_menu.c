#include "principal_menu.h"
#include <stdio.h>
#include <winsock2.h>
#include <windows.h> // para limpiar consola
#include <conio.h>   // para getch()
#include "codigosRet.h"
#include "principal_partida.h"
#include "interno_cliente.h"
#include "interno_jugador.h"

void menuPrincipal()
{
    int pos = 0;
    char tecla;
    char matPantalla[CANT_OPCIONES_PRINCIPAL][TAM_PAL_OPCION] =
    {
        "-> Nueva Partida",
        "   Ver Ranking",
        "   Dificultad",
        "   Salir",
    };
    SOCKET sock = clienteConectarAlServidor();

    puts("******LABERINTOS Y FANTASMAS******");
    putchar('\n');
    printf("\t- Presione enter -\t\n");
    tecla = getch();
    while (tecla != ENTER)
        tecla = getch();

    system("cls");
    mostrarMenu(matPantalla, CANT_OPCIONES_PRINCIPAL);

    while (!(pos == SALIR && tecla == ENTER))
    {
        tecla = getch();
        system("cls");

        if(tecla == ABAJO || tecla == ARRIBA)
        {
            actualizarMenu(matPantalla, CANT_OPCIONES_PRINCIPAL, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));
        }

        if (pos != SALIR && tecla == ENTER)
        {
            puts("Cargando...");
            if (pos == 0)
            {
                system("cls");
                configuracionPartida(sock);
            }

            else if(pos == 1)
            {
                system("cls");
                verRanking(sock);
            }

            else if(pos == 2)
            {
                system("cls");
                menuDeDificultad();
            }
        }

        mostrarMenu(matPantalla, CANT_OPCIONES_PRINCIPAL);
    }
}

int menuDeConfirmacion()
{
    int pos = 0;
    char tecla;
    char matrizPantalla[CANT_OPCIONES_CONFIRMACION][TAM_PAL_OPCION] =
    {
        "-> No",
        "   Si"
    };

    system("cls");
    puts("Seguro que desea salir? Su partida se tomara como perdida");
    mostrarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION);
    tecla = getch();

    while(tecla != ESC && tecla != ENTER)
    {
        if(tecla == ABAJO || tecla == ARRIBA)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));
        }

        system("cls");
        puts("Seguro que desea salir? Su partida se tomara como perdida");
        mostrarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION);
        tecla = getch();
    }

    if(tecla == ESC || (pos == 0 && tecla == ENTER))
    {
        return REANUDAR;
    }

    return TERMINAR;
}

int menuDePausa()
{
    int pos = 0;
    char tecla;
    char matrizPantalla[CANT_OPCIONES_PAUSA][TAM_PAL_OPCION] =
    {
        "-> Volver al juego",
        "   Salir al menu principal"
    };
    int salida = REANUDAR;

    system("cls");
    puts("*PAUSA*");
    mostrarMenu(matrizPantalla, CANT_OPCIONES_PAUSA);
    tecla = getch();
    while(salida == REANUDAR && !(tecla == ESC || (pos == 0 && tecla == ENTER)))
    {
        if(pos == 1 && tecla == ENTER)
        {
            salida = menuDeConfirmacion();
        }

        else if(tecla == ABAJO || tecla == ARRIBA)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_PAUSA, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));
        }

        if(salida == REANUDAR)
        {
            system("cls");
            puts("*PAUSA*");
            mostrarMenu(matrizPantalla, CANT_OPCIONES_PAUSA);
            tecla = getch();
        }
    }

    return salida;
}

int subMenuDeDificultad()
{
    int pos = OPCION_NORMAL;
    char tecla;
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

    system("cls");
    puts("DIFICULTAD");
    mostrarMenu(matrizPantalla, CANT_OPCIONES_DIFICULTAD);
    tecla = getch();

    while(tecla != ENTER)
    {
        if(tecla == ABAJO || tecla == ARRIBA)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_DIFICULTAD, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));
        }
        system("cls");
        puts("DIFICULTAD");
        mostrarMenu(matrizPantalla, CANT_OPCIONES_DIFICULTAD);
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

void menuDeDificultad()
{
    char tecla;
    int pos = 0;
    char matrizPantalla[CANT_OPCIONES_PAUSA][TAM_PAL_OPCION] =
    {
        "-> Seleccionar dificultad",
        "   Volver al menu principal"
    };

    system("cls");
    puts("DIFICULTAD");
    mostrarMenu(matrizPantalla, CANT_OPCIONES_PAUSA);
    tecla = getch();

    while(!(pos == 1 && tecla == ENTER))
    {
        if(pos == 0 && tecla == ENTER)
        {
            if(subMenuDeDificultad() == ERR_ARCHIVO)
            {
                fprintf(stderr, "Error en la apertura del archivo\n");
            }
        }

        if(tecla == ARRIBA || tecla == ABAJO)
        {
            actualizarMenu(matrizPantalla, CANT_OPCIONES_CONFIRMACION, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));
        }
        system("cls");
        puts("DIFICULTAD");
        mostrarMenu(matrizPantalla, CANT_OPCIONES_PAUSA);
        tecla = getch();
    }
    system("cls");
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

