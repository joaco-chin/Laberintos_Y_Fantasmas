#include "principal_partida.h"
#include <stdio.h>
#include <windows.h> // Para limpiar la consola system("cls")
#include <conio.h>   // para getch()
#include "codigosRet.h"
#include "principal_menu.h"
#include "interno_fantasma.h"
#include "interno_laberinto.h"
#include "estructuras_lista.h"

int partida(SOCKET sockCliente)
{
    tConfig conf;
    char linea[TAM_LINEA_CONF];
    char **matLab;
    int codigoDeError = TODO_OK;
    int resultado;
    tCola colaFantasmas;

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
    generarLaberintoAleatorio(matLab, conf.fil, conf.col, &conf.maxNumFantasmas, &conf.maxNumPremios, &conf.maxVidasExtra, &colaFantasmas);

    // guarda la matriz en "laberinto.txt"
    escribirMatrizEnArchivoTxt(matLab, "laberinto.txt", conf.fil, conf.col);

    resultado = loopPartida(matLab, &conf, sockCliente, &colaFantasmas);
    if(resultado == PARTIDA_PERDIDA)
    {
        puts("Game Over");
//        Sleep(500);
        limpiarConsola();
    }

    matrizDestruir((void **)matLab, conf.fil);
    colaVaciar(&colaFantasmas);
    return TODO_OK;
}

int loopPartida(char **matriz, tConfig *conf, SOCKET sockCliente, tCola* colaFantasmas)
{
    tJugador jug;
    int filaEntrada, columnaEntrada;
    int filaSalida, columnaSalida;
    char tecla;
    int salida = REANUDAR;
    tCola movimientos;
    tCola registro; // Cola para guardar el registro de los movimientos del jugador
    int cantMovimientos = 0;

    jug.vidas = conf->vidasInicio;
    jug.puntos = 0;

    matrizBuscar(matriz, 'E', &filaEntrada, &columnaEntrada, conf->fil, conf->col);
    jug.posFil = filaEntrada;
    jug.posCol = columnaEntrada;
    matrizBuscar(matriz, 'S', &filaSalida, &columnaSalida, conf->fil, conf->col);

    printf("Presione una tecla para comenzar, finaliza con ESC\n\n");
    matrizMostrar(matriz, conf->fil, conf->col);
    matrizRemplazarCaracterEnPosicion(matriz, 'J', jug.posFil, jug.posCol, conf->fil, conf->col);

    colaCrear(&movimientos);
    colaCrear(&registro);
//    tecla = ingresarTeclaDeJugador(VELOCIDAD_JUEGO);
    tecla = getch();

    while (salida != TERMINAR && matriz[jug.posFil][jug.posCol] != matriz[filaSalida][columnaSalida])
    {
        limpiarConsola();
        printf("Vidas: %d\n", jug.vidas);
        printf("Puntos: %d\n", jug.puntos);

        if(tecla != ESC)
        {

            if ((tecla == ABAJO || tecla == ARRIBA || tecla == IZQUIERDA || tecla == DERECHA))
            {
                matrizActualizarPosicionDeJugador(matriz, conf->fil, conf->col, &jug, jug.posFil + (tecla == ABAJO) - (tecla == ARRIBA), jug.posCol + (tecla == DERECHA) - (tecla == IZQUIERDA));
                colaEncolar(&registro, &jug, sizeof(tJugador));
            }

            calcularMovimientosFantasmas(matriz, conf->fil, conf->col, colaFantasmas, &jug, &movimientos);
            actualizarPosicionesFantasmas(matriz, conf->fil, conf->col, colaFantasmas, &jug, &movimientos);
            actualizarPuntosYVidas(&jug, matriz[jug.posFil][jug.posCol]);

            salida = matrizActualizarPorEstadoDeVidas(matriz, &jug, colaFantasmas, conf, filaEntrada, columnaEntrada);
            matrizMostrar(matriz, conf->fil, conf->col);
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
        {
//            tecla = ingresarTeclaDeJugador(VELOCIDAD_JUEGO);
            tecla = getch();
        }
    }
    limpiarConsola();

    if (salida == TERMINAR)
    {
        colaVaciar(&movimientos);
        colaVaciar(&registro);
        return PARTIDA_PERDIDA;
    }

    // ya se tiene puntaje, contar cant de movimientos del jugador que se guardaron en la cola
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
    colaVaciar(&movimientos);
    colaVaciar(&registro);
    return PARTIDA_GANADA;
}

int matrizActualizarPorEstadoDeVidas(char **matriz, tJugador *jug, tCola* colaFantasmas, tConfig *conf, int filaEntrada, int columnaEntrada)
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

        // Hacemos que los fantasmas retrocedan hacia su posicion inicial
        while(colaDesencolar(colaFantasmas, &fantasma, sizeof(tFantasma)) == TODO_OK)
        {
            fantasma.fil = fantasma.posInicial.fila;
            fantasma.col = fantasma.posInicial.columna;
            fantasma.caracterAnterior = CAMINO;
            colaEncolar(&aux, &fantasma, sizeof(tFantasma));
        }

        while(colaDesencolar(&aux, &fantasma, sizeof(tFantasma)) == TODO_OK)
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
