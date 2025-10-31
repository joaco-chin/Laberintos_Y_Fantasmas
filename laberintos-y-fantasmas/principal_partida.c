#include "principal_partida.h"
#include <stdio.h>
#include <windows.h> // Para limpiar la consola system("cls")
#include "codigosRet.h"
#include "principal_menu.h"
#include "interno_fantasma.h"
#include "estructuras_lista.h"

int configuracionPartida(SOCKET sockCliente)
{
    tConfig conf;
    char linea[TAM_LINEA_CONF];
    char **matLab;
    int codigoDeError = TODO_OK;
    int resultado;
    tCola colaFantasmas;
    tPosicion entradaYSalida[2];

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
    colaCrear(&colaFantasmas); // Creamos la lista de fantasmas para enviarla como parametro a la generacion del laberinto, para poder cargar las posiciones iniciales de cada fantasma
    generarLaberintoAleatorio(matLab, conf.fil, conf.col, &conf.maxNumFantasmas, &conf.maxNumPremios,
                              &conf.maxVidasExtra, &colaFantasmas, entradaYSalida);

    // guarda la matriz en "laberinto.txt"
    escribirMatrizEnArchivoTxt(matLab, "laberinto.txt", conf.fil, conf.col);

    resultado = ejecucionPartida(matLab, &conf, sockCliente, &colaFantasmas, entradaYSalida);
    if (resultado == PARTIDA_PERDIDA)
    {
        clear();
        mvprintw(0, 0, "- Game Over -");
        refresh();
        napms(TIEMPO_MENSAJE);
    }

    matrizDestruir((void **)matLab, conf.fil);
    colaVaciar(&colaFantasmas);
    return TODO_OK;
}
//
// int partida_ejecucion(char **matriz, tConfig *conf, SOCKET sockCliente, tCola* colaFantasmas)
//{
//    tJugador jug;
//    int filaEntrada, columnaEntrada;
//    int filaSalida, columnaSalida;
//    char tecla;
//    int salida = REANUDAR;
//    tCola movimientos; // Cola para guardar los movimientos de la m�quina
//    tCola registro; // Lista para guardar el registro de los movimientos del jugador
//    int cantMovimientos = 0;
//
//    jug.vidas = conf->vidasInicio;
//    jug.puntos = 0;
//
//    matrizBuscar(matriz, 'E', &filaEntrada, &columnaEntrada, conf->fil, conf->col);
//    jug.posFil = filaEntrada;
//    jug.posCol = columnaEntrada;
//    matrizBuscar(matriz, 'S', &filaSalida, &columnaSalida, conf->fil, conf->col);
//
//    printf("Presione una tecla para comenzar, finaliza con ESC\n\n");
//    matrizMostrar(matriz, conf->fil, conf->col);
//    matrizRemplazarCaracterEnPosicion(matriz, 'J', jug.posFil, jug.posCol, conf->fil, conf->col);
//
//    colaCrear(&movimientos);
//    colaCrear(&registro);
////    tecla = ingresarTeclaDeJugador(VELOCIDAD_JUEGO);
//    tecla = (char)ingresarMovimiento();
//
//    while (salida != TERMINAR && matriz[jug.posFil][jug.posCol] != matriz[filaSalida][columnaSalida])
//    {
//        if(tecla != ESC)
//        {
//            if(ES_MOVIMIENTO(tecla))
//            {
//                matrizActualizarPosicionDeJugador(matriz, conf->fil, conf->col, &jug,
//                jug.posFil + (tecla == ABAJO) - (tecla == ARRIBA), jug.posCol + (tecla == DERECHA) - (tecla == IZQUIERDA));
//                colaEncolar(&registro, &jug, sizeof(tJugador));
//            }
//
//            calcularMovimientosFantasmas(matriz, conf->fil, conf->col, colaFantasmas, &jug, &movimientos);
//            actualizarPosicionesFantasmas(matriz, conf->fil, conf->col, colaFantasmas, &jug, &movimientos);
//            actualizarPuntosYVidas(&jug, matriz[jug.posFil][jug.posCol]);
//
//            salida = actualizarJuegoPorEstadoDeVidas(matriz, &jug, colaFantasmas, conf, filaEntrada, columnaEntrada);
//            dibujarPantalla(matriz, conf->fil, conf->col, jug.vidas, jug.puntos);
//        }
//        else
//        {
//            salida = menuDePausa();
//            dibujarPantalla(matriz, conf->fil, conf->col, jug.vidas, jug.puntos);
//        }
//
//        if(salida != TERMINAR)
//        {
//            tecla = ingresarTeclaDeJugador(VELOCIDAD_JUEGO); // Ingresa la tecla con un timer
////            tecla = (char)ingresarMovimiento();
//        }
//    }
//    system("cls");
//
//    if (salida == TERMINAR)
//    {
//        colaVaciar(&movimientos);
//        colaVaciar(&registro);
//        return PARTIDA_PERDIDA;
//    }
//
//    // ya se tiene puntaje, contar cant de movimientos del jugador que se guardaron en la cola
//    printf("Partida ganada! Puntos: %d, Movimientos: %d\n", jug.puntos, cantMovimientos);
//    Sleep(TIEMPO_MENSAJE);
//
//    if (sockCliente != INVALID_SOCKET)
//    {
//        char mensaje[BUFFER_SIZE];
//        sprintf(mensaje, "GUARDAR_PUNTUACION|%d|%d", jug.puntos, cantMovimientos);
//        char respuesta[BUFFER_SIZE];
//        if (enviarPeticion(sockCliente, mensaje, respuesta) == 0)
//            printf("[Servidor]: %s\n", respuesta);
//        else
//            printf("Error al enviar o recibir datos del servidor.\n");
//    }
//    else
//    {
//        printf("No se pudo guardar la puntuacion, no hay conexion con el servidor.\n");
//    }
//
//    system("cls");
//    colaVaciar(&movimientos);
//    colaVaciar(&registro);
//    return PARTIDA_GANADA;
//}

int ejecucionPartida(char **matriz, tConfig *conf, SOCKET sockCliente, tCola *colaFantasmas, tPosicion entradaYSalida[])
{
    tJugador jug = {entradaYSalida[0].fila, entradaYSalida[0].columna, conf->vidasInicio, 0, 0};
    //|jug.fil = -1|jug.col = -1|jug.vidas = conf->vidasInicio|jug.puntos = 0 |jug.cantMovimientos = 0|
    int salida = REANUDAR;
    tCola movimientos; // Cola para guardar los movimientos de la m�quina
    tCola registro;    // Cola para guardar el registro de los movimientos del jugador
    int bonificacion = determinarBonificacion(conf->dificultad);

    dibujarInicioPartida(matriz, conf->fil, conf->col);
    matrizRemplazarCaracterEnPosicion(matriz, JUGADOR, jug.posFil, jug.posCol, conf->fil, conf->col);
    colaCrear(&movimientos);
    colaCrear(&registro);

    while (salida != TERMINAR && matriz[jug.posFil][jug.posCol] != matriz[entradaYSalida[1].fila][entradaYSalida[1].columna])
    {
        salida = procesarAccionDeJugador(matriz, conf->fil, conf->col, &jug, &registro);

        if (salida == REANUDAR)
        {
            salida = procesarEventosDePartida(matriz, conf, &jug, colaFantasmas, &movimientos, entradaYSalida);
            dibujarPartida(matriz, conf->fil, conf->col, conf->dificultad, jug.vidas, jug.puntos);
        }

    }

    if (salida == TERMINAR)
    {
        colaVaciar(&movimientos);
        colaVaciar(&registro);
        return PARTIDA_PERDIDA;
    }

    clear();
    mvprintw(0, 0, "Partida ganada! Puntos: %d|Bonificacion: %d|Total: %d|Movimientos: %d", jug.puntos, bonificacion, jug.puntos * bonificacion, jug.cantMovimientos);
    refresh();
    napms(TIEMPO_MENSAJE);

//    dibujarMensaje("Partida ganada! Puntos: %d|Bonificacion: %d|Total: %d|Movimientos: %d");

    if (sockCliente != INVALID_SOCKET)
    {
        char mensaje[BUFFER_SIZE];
        sprintf(mensaje, "GUARDAR_PUNTUACION|%d|%d|%s", jug.puntos * bonificacion, jug.cantMovimientos, conf->dificultad);
        char respuesta[BUFFER_SIZE];
        char tecla;
        if (enviarPeticion(sockCliente, mensaje, respuesta) == 0)
        {
            printf("[Servidor]: %s\n", respuesta);
            printf("->  Volver");
            tecla = getch();
            while (tecla != ENTER)
            {
                tecla = getch();
            }
        }
        else
            printf("Error al enviar o recibir datos del servidor.\n");
    }
    else
    {
//        clear();
//        mvprintw(0, 0, "No se pudo guardar la puntuacion, no hay conexion con el servidor.\n");
//        refresh();
//        napms(TIEMPO_MENSAJE);
//        dibujarMensaje("No se pudo guardar la puntuacion, no hay conexion con el servidor");
    }

    colaVaciar(&movimientos);
    colaVaciar(&registro);
    return PARTIDA_GANADA;
}

int procesarAccionDeJugador(char **matriz, int cf, int cc, tJugador *jug, tCola *registro)
{
//    char tecla = ingresarTeclaDeJugador(TIEMPO_INPUT);
    int tecla = getch();

    if(tecla == ESC)
    {
        if (menuDePausa() != REANUDAR)
        {
            return TERMINAR;
        }
    }
    else if (ES_MOVIMIENTO(tecla))
    {
        matrizActualizarPosicionDeJugador(matriz, cf, cc, jug,
                                          jug->posFil + (tecla == KEY_DOWN) - (tecla == KEY_UP), jug->posCol + (tecla == KEY_RIGHT) - (tecla == KEY_LEFT));
        jug->cantMovimientos++;
        colaEncolar(registro, jug, sizeof(tJugador));
    }

    return REANUDAR;
}

int procesarEventosDePartida(char **matriz, tConfig *conf, tJugador *jug, tCola *colaFantasmas, tCola *movimientos, tPosicion entradaYSalida[])
{
    calcularMovimientosFantasmas(matriz, conf->fil, conf->col, colaFantasmas, jug, movimientos);
    actualizarPosicionesFantasmas(matriz, conf->fil, conf->col, colaFantasmas, jug, movimientos);
    actualizarPuntosYVidas(jug, matriz[jug->posFil][jug->posCol]);
    if (actualizarPartidaPorEstadoDeVidas(matriz, jug, colaFantasmas, conf, entradaYSalida[0].fila, entradaYSalida[0].columna) != REANUDAR)
    {
        return TERMINAR;
    }
    return REANUDAR;
}

int actualizarPartidaPorEstadoDeVidas(char **matriz, tJugador *jug, tCola *colaFantasmas, tConfig *conf, int filaEntrada, int columnaEntrada)
{
    tFantasma fantasma;
    tCola aux;

    colaCrear(&aux);
    if (jug->vidas < conf->vidasInicio)
    {
        matriz[jug->posFil][jug->posCol] = CAMINO;
        jug->posFil = filaEntrada;
        jug->posCol = columnaEntrada;
        matriz[jug->posFil][jug->posCol] = JUGADOR;
        conf->vidasInicio = jug->vidas;

        while (colaDesencolar(colaFantasmas, &fantasma, sizeof(tFantasma)) == TODO_OK)
        {
            matriz[fantasma.fil][fantasma.col] = fantasma.caracterAnterior;
            fantasma.fil = fantasma.posInicial.fila;
            fantasma.col = fantasma.posInicial.columna;
            fantasma.caracterAnterior = CAMINO;
            fantasma.estaVivo = FANTASMA_VIVO;
            colaEncolar(&aux, &fantasma, sizeof(tFantasma));
        }

        while (colaDesencolar(&aux, &fantasma, sizeof(tFantasma)) == TODO_OK)
        {
            colaEncolar(colaFantasmas, &fantasma, sizeof(tFantasma));
        }
    }
    else if (jug->vidas > conf->vidasInicio)
        conf->vidasInicio = jug->vidas;
    else if (jug->vidas <= 0)
    {
        return TERMINAR;
    }

    return REANUDAR;
}

int determinarBonificacion(const char *dif)
{
    if (strcmpi(dif, "NORMAL") == 0)
    {
        return BONIFICACION_NORMAL;
    }
    else if (strcmpi(dif, "DIFICIL") == 0)
    {
        return BONIFICACION_DIFICIL;
    }
    else if (strcmpi(dif, "PESADILLA") == 0)
    {
        return BONIFICACION_PESADILLA;
    }

    return BONIFICACION_FACIL;
}

void verRanking(SOCKET sockCliente)
{
    char respuesta[BUFFER_SIZE];
    char tecla;

    if (sockCliente != INVALID_SOCKET)
    {
        if (enviarPeticion(sockCliente, "VER_RANKING", respuesta) == 0)
        {
            printf("[Servidor]:\n%s\n", respuesta);
            printf("->  Volver");
            tecla = getch();
            while (tecla != ENTER)
            {
                tecla = getch();
            }
        }
        else
        {
            printf("Error al enviar o recibir datos del servidor.\n");
            Sleep(TIEMPO_MENSAJE);
        }
    }
    else
    {
        printf("No se puede ver el ranking, no hay conexion con el servidor.\n");
        Sleep(TIEMPO_MENSAJE);
    }
    system("cls");
}
