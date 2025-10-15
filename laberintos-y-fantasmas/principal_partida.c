#include "principal_partida.h"
#include <stdio.h>
#include <windows.h> // Para limpiar la consola system("cls")
#include <conio.h>   // para getch()
#include "codigosRet.h"
#include "interno_jugador.h"
#include "interno_fantasma.h"
#include "principal_menu.h"
#include "interno_laberinto.h"

int partida()
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

    // crearLaberintoEnTxt()-> conf: contiene filas, columnas, maxNumFantasmas, maxNumPremios, maxVidasExtra REALES generados en el laberinto
    generarLaberintoAleatorio(matLab, conf.fil, conf.col, &conf.maxNumFantasmas, &conf.maxNumPremios, &conf.maxVidasExtra);

    // guarda la matriz en "laberinto.txt"
    escribirMatrizEnArchivoTxt(matLab, "laberinto.txt", conf.fil, conf.col);

    codigoDeError = loopPartida(matLab, &conf);
    if (codigoDeError != TODO_OK)
    {
        matrizDestruir((void **)matLab, conf.fil);
        return codigoDeError;
    }

    matrizDestruir((void **)matLab, conf.fil);
    return TODO_OK;
}

int loopPartida(char **matriz, tConfig *conf)
{
    tJugador jug;
    int filaEntrada, columnaEntrada;
    int filaSalida, columnaSalida;
    char tecla;
    tFantasma* fantasmas = malloc(conf->maxNumFantasmas * sizeof(tFantasma));
    int cantFantasmas = conf->maxNumFantasmas;
    if(!fantasmas)
    {
        return SIN_MEM;
    }

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
    while (tecla != ESC && matriz[jug.posFil][jug.posCol] != matriz[filaSalida][columnaSalida])
    {
        limpiarConsola();
        printf("Vidas: %d\n", jug.vidas);
        printf("Puntos: %d\n", jug.puntos);

        if ((tecla == ABAJO || tecla == ARRIBA || tecla == IZQUIERDA || tecla == DERECHA))
            matrizActualizarPosicionDeJugador(matriz, conf->fil, conf->col, &jug, jug.posFil + (tecla == ABAJO) - (tecla == ARRIBA), jug.posCol + (tecla == DERECHA) - (tecla == IZQUIERDA));

        matrizMostrar(matriz, conf->fil, conf->col);

        matrizActualizarPorEstadoDeVidas(matriz, &jug, conf, filaEntrada, columnaEntrada, &tecla);

        // poner un timer si no se preciona tecla durante cierto tiempo pasa un ciclo y se mueven los fantasmas con el jugador quieto
        if (tecla != ESC)
            tecla = getch();
    }
    limpiarConsola();

    if (tecla == ESC)
        return PARTIDA_PERDIDA;

    return PARTIDA_GANADA;
}
