#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include "principal_menu.h"
#include "principal_partida.h"
#include "interno_cliente.h"

void juego()
{
    SOCKET sock = clienteConectarAlServidor();

    int opcion;

    while ((opcion = menu()) != SALIR)
    {
        switch (opcion)
        {
        case NUEVA_PARTIDA:
            partida(sock);
            break;

        case VER_RANKING:
            verRanking(sock);
            break;
        }
    }

    cerrarConexion(sock);
}
