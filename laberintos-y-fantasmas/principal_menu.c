#include "principal_menu.h"
#include <stdio.h>
#include <windows.h> // para limpiar consola
#include <conio.h>   // para getch()
#include "codigosRet.h"
#include "interno_jugador.h"

int menu()
{
    int pos = 0;
    char tecla;
    char matPantalla[CANT_OPCIONES][TAM_PAL] =
    {
        "-> Nueva Partida",
        "   Ver Ranking",
        "   Salir",
    };

    puts("******LABERINTOS Y FANTASMAS******");
    putchar('\n');
    printf("\t- Presione enter -\t\n");
    tecla = getch();
    while (tecla != ENTER)
        tecla = getch();

    system("cls");

    mostrarMenu(matPantalla, CANT_OPCIONES);

    while (!(pos == SALIR && tecla == ENTER))
    {
        tecla = getch();
        system("cls");

        if (tecla == ABAJO || tecla == ARRIBA)
            actualizarMenu(matPantalla, CANT_OPCIONES, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));

        mostrarMenu(matPantalla, CANT_OPCIONES);

        if (pos != SALIR && tecla == ENTER)
        {
            puts("Cargando...");
            if (pos == 0)
            {
                system("cls");
                return NUEVA_PARTIDA;
            }

            else
            {
                system("cls");
                return VER_RANKING;
            }
        }
    }

    return SALIR;
}

int menuDePausa()
{
    int pos = 0;
    char tecla;
    char matrizPantalla[CANT_OPCIONES_PAUSA][TAM_PAL] =
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

int menuDeConfirmacion()
{
    int pos = 0;
    char tecla;
    char matrizPantalla[CANT_OPCIONES_CONFIRMACION][TAM_PAL] =
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

void actualizarMenu(char matriz[][TAM_PAL], int cf, int *posAct, int posNueva)
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

void mostrarMenu(char matriz[][TAM_PAL], int filas)
{
    int i;

    for (i = 0; i < filas; i++)
    {
        puts(matriz[i]);
        putchar('\n');
    }
}

//void limpiarConsola() // lo mismo que poner system("cls");
//{
//    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    DWORD count;
//    DWORD cellCount;
//    COORD homeCoords = {0, 0};
//
//    if (hConsole == INVALID_HANDLE_VALUE)
//        return;
//
//    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
//        return;
//    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
//
//    if (!FillConsoleOutputCharacter(
//                hConsole,
//                (TCHAR)' ',
//                cellCount,
//                homeCoords,
//                &count))
//        return;
//
//    SetConsoleCursorPosition(hConsole, homeCoords);
//}
