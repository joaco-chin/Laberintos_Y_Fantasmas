#include "principal_partida.h"
#include <stdio.h>
#include "codigosRet.h"
#include "principal_menu.h"
#include "interno_fantasma.h"
#include "estructuras_lista.h"

int configuracionPartida(SOCKET sockCliente, int altoStdscr, int anchoStdscr)
{
    tConfig conf;
    char linea[TAM_LINEA_CONF];
    char **matLab;
    int codigoDeError = TODO_OK;
    int resultado;
    tLista fantasmas;
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
    listaCrear(&fantasmas); // Creamos la lista de fantasmas para enviarla como parametro a la generacion del laberinto, para poder cargar las posiciones iniciales de cada fantasma
    generarLaberintoAleatorio(matLab, conf.fil, conf.col, &conf.maxNumFantasmas, &conf.maxNumPremios,
                              &conf.maxVidasExtra, &fantasmas, entradaYSalida);

    // guarda la matriz en "laberinto.txt"
    escribirMatrizEnArchivoTxt(matLab, "laberinto.txt", conf.fil, conf.col);

    resultado = ejecucionPartida(matLab, &conf, sockCliente, &fantasmas, entradaYSalida, anchoStdscr, altoStdscr);
    if (resultado == PARTIDA_PERDIDA)
    {
        clear();
        mvprintw(0, 0, "- Game Over -");
        refresh();
        napms(TIEMPO_MENSAJE);
    }

    matrizDestruir((void **)matLab, conf.fil);
    listaVaciar(&fantasmas);
    return TODO_OK;
}

int ejecucionPartida(char **matriz, tConfig *conf, SOCKET sockCliente, tLista *fantasmas, tPosicion entradaYSalida[], int altoStdscr, int anchoStdscr)
{
    tJugador jug;
    int salida = REANUDAR;
    tCola movimientos; // Cola para guardar los movimientos de la partida
    tLista registro;    // Cola para guardar el registro de los movimientos del jugador // CAMBIAR POR UNA LISTA
    int bonificacion = determinarBonificacion(conf->dificultad);

    jug.inGame.y = entradaYSalida[0].fila;
    jug.inGame.x = entradaYSalida[0].columna;
    jug.inGame.caracterEnt = JUGADOR;
    jug.inGame.caracterFondo = CAMINO;
    jug.inGame.estaVivo = 1;
    jug.inGame.posInicial = entradaYSalida[0];
    jug.vidas = conf->vidasInicio;
    jug.puntos = 0;
    jug.cantMovimientos = 0;

    dibujarInicioPartida(matriz, conf->fil, conf->col);
    matrizRemplazarCaracterEnPosicion(matriz, jug.inGame.caracterFondo, jug.inGame.y, jug.inGame.x, conf->fil, conf->col);
    colaCrear(&movimientos);
    listaCrear(&registro);

    while(salida != TERMINAR && matriz[jug.inGame.y][jug.inGame.x] != matriz[entradaYSalida[1].fila][entradaYSalida[1].columna])
    {
        salida = procesarAccionDeJugador(matriz, conf->fil, conf->col, &jug, &registro, &movimientos, altoStdscr, anchoStdscr);

        if(salida == REANUDAR)
        {
            salida = procesarEventosDePartida(matriz, conf, &jug, fantasmas, &movimientos, entradaYSalida);
            dibujarPartida(matriz, conf->fil, conf->col, conf->dificultad, jug.vidas, jug.puntos);
        }

    }

    if(salida == TERMINAR)
    {
        colaVaciar(&movimientos);
        listaVaciar(&registro);
        return PARTIDA_PERDIDA;
    }

    clear();
    printw("Partida ganada! Puntos: %d|Bonificacion: %d|Total: %d|Movimientos: %d\n", jug.puntos, bonificacion, jug.puntos * bonificacion, jug.cantMovimientos);
    printw("Movimientos realizados:\n");
    listaRecorrer(&registro, impPosEnPantalla);
    refresh();
    napms(TIEMPO_MENSAJE * 10);

    if (sockCliente != INVALID_SOCKET)
    {
        char mensaje[BUFFER_SIZE];
        sprintf(mensaje, "GUARDAR_PUNTUACION|%d|%d|%s", jug.puntos * bonificacion, jug.cantMovimientos, conf->dificultad);
        char respuesta[BUFFER_SIZE];
        int tecla;
        if (enviarPeticion(sockCliente, mensaje, respuesta) == 0)
        {
            clear();
            mvprintw(0, 0, "[Servidor]: %s\n", respuesta);
            mvprintw(0, 0, "->  Volver");
            tecla = getch();
            while (tecla != ENTER)
            {
                tecla = getch();
            }
        }
        else
            mvprintw(0, 0, "Error al enviar o recibir datos del servidor.\n");

        refresh();
    }
    else
    {
        clear();
        mvprintw(0, 0, "No se pudo guardar la puntuacion, no hay conexion con el servidor.\n");
        refresh();
        napms(TIEMPO_MENSAJE);
//        dibujarMensaje("No se pudo guardar la puntuacion, no hay conexion con el servidor");
    }

    colaVaciar(&movimientos);
    listaVaciar(&registro);
    return PARTIDA_GANADA;
}

int procesarAccionDeJugador(char **matriz, int cf, int cc, tJugador *jug, tLista *registro, tCola *movimientos, int altoStdscr, int anchoStdscr)
{
    int tecla = getch();
    tPosicion posJugador;

    if(tecla == ESC)
    {
        if(menuDePausa(altoStdscr, anchoStdscr) != REANUDAR)
        {
            return TERMINAR;
        }
    }
    else if(ES_MOVIMIENTO(tecla))
    {
//        matrizActualizarPosicionDeJugador(matriz, cf, cc, jug,
//                                          jug->posFil + (tecla == KEY_DOWN) - (tecla == KEY_UP), jug->posCol + (tecla == KEY_RIGHT) - (tecla == KEY_LEFT));
        moverJugador(matriz, cf, cc, jug, jug->inGame.y + (tecla == KEY_DOWN) - (tecla == KEY_UP),
        jug->inGame.x + (tecla == KEY_RIGHT) - (tecla == KEY_LEFT), movimientos);

        jug->cantMovimientos++;
        posJugador.fila = jug->inGame.y;
        posJugador.columna = jug->inGame.x;
        listaPonerAlFinalREVISAR(registro, &posJugador, sizeof(tPosicion));
    }

    return REANUDAR;
}

void desencolarMovimientosPartida(char** matriz, int cf, int cc, tCola* movimientos, tLista* fantasmas)
{
    tEntidad entidad, aux;
    int pos;

    while(colaDesencolar(movimientos, &entidad, sizeof(tEntidad)) == TODO_OK)
    {
        pos = listaBuscarPorClave(fantasmas, &entidad, cmpPosIniciales);
        if(pos != NO_ENCONTRADO)
        {
            listaRemoverPorPos(fantasmas, &aux, sizeof(tEntidad), pos);
            matriz[aux.y][aux.x] = entidad.caracterFondo;
            if(!(aux.caracterEnt == FANTASMA && matriz[entidad.y][entidad.x] == JUGADOR))
            {
                listaInsertarAlInicio(fantasmas, &entidad, sizeof(tEntidad));
            }
            else
            {
                entidad.estaVivo = FANTASMA_MUERTO;
            }
        }

        if(matriz[entidad.y][entidad.x] != FANTASMA)
        {
            entidad.caracterFondo = matriz[entidad.y][entidad.x];
        }

        if(entidad.caracterEnt == JUGADOR && matriz[entidad.y][entidad.x] == FANTASMA)
        {
            pos = listaBuscarPorClave(fantasmas, &entidad, cmpPosEntidad);
            listaRemoverPorPos(fantasmas, &aux, sizeof(tEntidad), pos);
        }

        matrizRemplazarCaracterEnPosicion(matriz, entidad.caracterEnt, entidad.y, entidad.x, cf, cc);
    }
}

int procesarEventosDePartida(char **matriz, tConfig *conf, tJugador *jug, tLista *fantasmas, tCola *movimientos, tPosicion entradaYSalida[])
{
    calcularMovimientosFantasmas(matriz, conf->fil, conf->col, fantasmas, jug, movimientos);
    desencolarMovimientosPartida(matriz, conf->fil, conf->col, movimientos, fantasmas);
    actualizarPuntosYVidas(jug, matriz);
    if(actualizarPartidaPorEstadoDeVidas(matriz, jug, fantasmas, conf, entradaYSalida[0].fila, entradaYSalida[0].columna) != REANUDAR)
    {
        return TERMINAR;
    }
    return REANUDAR;
}

int actualizarPartidaPorEstadoDeVidas(char **matriz, tJugador *jug, tLista *fantasmas, tConfig *conf, int filaEntrada, int columnaEntrada)
{
    if(jug->vidas < conf->vidasInicio)
    {
        matriz[jug->inGame.y][jug->inGame.x] = CAMINO;
        jug->inGame.y = filaEntrada;
        jug->inGame.x = columnaEntrada;
        matriz[jug->inGame.y][jug->inGame.x] = JUGADOR;
        conf->vidasInicio = jug->vidas;
        limpiarFantasmas(matriz, fantasmas);
        listaMap(fantasmas, resetearFantasma);
    }
    else if (jug->vidas > conf->vidasInicio)
    {
        conf->vidasInicio = jug->vidas;
    }
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
    int tecla;

    clear();
    if (sockCliente != INVALID_SOCKET)
    {
        if (enviarPeticion(sockCliente, "VER_RANKING", respuesta) == 0)
        {
            mvprintw(0, 0, "[Servidor]:\n%s\n", respuesta);
            mvprintw(0, 0, "->  Volver");
            refresh();
            tecla = getch();
            while (tecla != ENTER)
            {
                tecla = getch();
            }
        }
        else
        {
            fprintf(stderr, "Error al enviar o recibir datos del servidor.\n");
        }
    }
    else
    {
        mvprintw(0, 0, "Error al enviar o recibir datos del servidor.\n");
        refresh();
        napms(TIEMPO_MENSAJE);
    }
}
