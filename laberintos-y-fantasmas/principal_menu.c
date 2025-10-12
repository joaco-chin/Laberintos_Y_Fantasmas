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

    limpiarConsola();

    mostrarMenu(matPantalla, CANT_OPCIONES);

    while (!(pos == SALIR && tecla == ENTER))
    {
        tecla = getch();
        limpiarConsola();

        if (tecla == ABAJO || tecla == ARRIBA)
            actualizarMenu(matPantalla, &pos, pos + (tecla == ABAJO) - (tecla == ARRIBA));

        mostrarMenu(matPantalla, CANT_OPCIONES);

        if (pos != SALIR && tecla == ENTER)
        {
            puts("Cargando...");
            if (pos == 0)
            {
                limpiarConsola();
                return NUEVA_PARTIDA;
            }

            else
            {
                limpiarConsola();
                return VER_RANKING;
            }
        }
    }

    return SALIR;
}

void actualizarMenu(char matriz[CANT_OPCIONES][TAM_PAL], int *posAct, int posNueva)
{
    int j = 0;

    if (posNueva >= 0 && posNueva < CANT_OPCIONES)
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

void mostrarMenu(char matriz[CANT_OPCIONES][TAM_PAL], int filas)
{
    int i;

    for (i = 0; i < filas; i++)
    {
        puts(matriz[i]);
        putchar('\n');
    }
}

void limpiarConsola() // lo mismo que poner system("cls");
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(
            hConsole,
            (TCHAR)' ',
            cellCount,
            homeCoords,
            &count))
        return;

    SetConsoleCursorPosition(hConsole, homeCoords);
}
