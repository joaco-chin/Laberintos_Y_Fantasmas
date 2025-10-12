#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include "principal_menu.h"
#include "principal_partida.h"

void juego()
{
    int opcion;

    while ((opcion = menu()) != SALIR)
    {
        switch (opcion)
        {
        case NUEVA_PARTIDA:
            partida();
            break;

        case VER_RANKING:
            printf("RANKING:\n");
            break;
        }
    }
}
