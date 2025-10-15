#include "principal_partida.h"
#include <stdio.h>
#include <windows.h> // Para limpiar la consola system("cls")
#include <conio.h>   // para getch()
#include "codigosRet.h"
#include "interno_jugador.h"
#include "interno_fantasma.h"
#include "principal_menu.h"
#include "interno_laberinto.h"

int partida(SOCKET sockCliente)
{
    tConfig conf;
    char linea[TAM_LINEA_CONF];
    char **matLab;
    int codigoDeError = TODO_OK;

    // inicializa configuracion desde "config.txt"
    if ((codigoDeError = inicializarConfConTxt(linea, &conf, "config.txt")) != TODO_OK)
    {
        puts("Error al trozar el archivo de configuracion");
        return codigoDeError;
    }

    // crea la matriz
    matLab = (char **)matrizCrear(sizeof(char), conf.fil, conf.col);
    if (matLab == NULL)
    {
        puts("Sin memoria");
        return SIN_MEM;
    }

    // generarLaberintoAleatorio-> conf: contiene filas, columnas, maxNumFantasmas, maxNumPremios, maxVidasExtra REALES generados en el laberinto
    generarLaberintoAleatorio(matLab, conf.fil, conf.col, &conf.maxNumFantasmas, &conf.maxNumPremios, &conf.maxVidasExtra);

    // guarda la matriz en "laberinto.txt"
    escribirMatrizEnArchivoTxt(matLab, "laberinto.txt", conf.fil, conf.col);

    codigoDeError = loopPartida(matLab, &conf, sockCliente);

    if (codigoDeError != TODO_OK)
    {
        matrizDestruir((void **)matLab, conf.fil);
        return codigoDeError;
    }

    matrizDestruir((void **)matLab, conf.fil);
    return TODO_OK;
}

int loopPartida(char **matriz, tConfig *conf, SOCKET sockCliente)
{
    tJugador jug;
    int filaEntrada, columnaEntrada;
    int filaSalida, columnaSalida;
    char tecla;
    int salida = REANUDAR;

    jug.vidas = conf->vidasInicio;
    jug.puntos = 0;

    matrizBuscar(matriz, 'E', &filaEntrada, &columnaEntrada, conf->fil, conf->col);
    jug.posFil = filaEntrada;
    jug.posCol = columnaEntrada;
    matrizBuscar(matriz, 'S', &filaSalida, &columnaSalida, conf->fil, conf->col);

    printf("Presione una tecla para comenzar, finaliza con ESC\n\n");
    matrizMostrar(matriz, conf->fil, conf->col);
    matrizRemplazarCaracterEnPosicion(matriz, 'J', jug.posFil, jug.posCol, conf->fil, conf->col);

    tecla = getch();
    while (salida != TERMINAR && matriz[jug.posFil][jug.posCol] != matriz[filaSalida][columnaSalida])
    {
        limpiarConsola();
        printf("Vidas: %d\n", jug.vidas);
        printf("Puntos: %d\n", jug.puntos);

        if(tecla != ESC)
        {
            if ((tecla == ABAJO || tecla == ARRIBA || tecla == IZQUIERDA || tecla == DERECHA))
                matrizActualizarPosicionDeJugador(matriz, conf->fil, conf->col, &jug, jug.posFil + (tecla == ABAJO) - (tecla == ARRIBA), jug.posCol + (tecla == DERECHA) - (tecla == IZQUIERDA));

            matrizMostrar(matriz, conf->fil, conf->col);

            salida = matrizActualizarPorEstadoDeVidas(matriz, &jug, conf, filaEntrada, columnaEntrada);
        }
        else
        {
            salida = menuDePausa();
            limpiarConsola();
            printf("Vidas: %d\n", jug.vidas);
            printf("Puntos: %d\n", jug.puntos);
            matrizMostrar(matriz, conf->fil, conf->col);
        }

        // poner un timer si no se preciona tecla durante cierto tiempo pasa un ciclo y se mueven los fantasmas con el jugador quieto
        if(salida != TERMINAR)
            tecla = getch();
    }
    limpiarConsola();

    if (salida == TERMINAR)
        return PARTIDA_PERDIDA;

    // ya se tiene puntaje, contar cant de movimientos del jugador que se guardaron en la cola
    int cantMovimientos = 0;
    printf("Partida ganada! Puntos: %d, Movimientos: %d\n", jug.puntos, cantMovimientos);

    if (sockCliente != INVALID_SOCKET)
    {
        char mensaje[BUFFER_SIZE];
        sprintf(mensaje, "GUARDAR_PUNTUACION|%d|%d", jug.puntos, cantMovimientos);
        char respuesta[BUFFER_SIZE];
        if (enviarPeticion(sockCliente, mensaje, respuesta) == 0)
            printf("[Servidor]: %s\n", respuesta);
        else
            printf("Error al enviar o recibir datos del servidor.\n");
    }
    else
    {
        printf("No se pudo guardar la puntuacion, no hay conexion con el servidor.\n");
    }

//    limpiarConsola();
    return PARTIDA_GANADA;
}

void verRanking(SOCKET sockCliente)
{
    if (sockCliente != INVALID_SOCKET)
    {
        char respuesta[BUFFER_SIZE];
        if (enviarPeticion(sockCliente, "VER_RANKING", respuesta) == 0)
            printf("[Servidor]: %s\n", respuesta);
        else
            printf("Error al enviar o recibir datos del servidor.\n");
    }
    else
    {
        printf("No se puede ver el ranking, no hay conexion con el servidor.\n");
    }
    limpiarConsola();
}
