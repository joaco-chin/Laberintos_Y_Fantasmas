#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "principal_archivo.h"
#include "interno_matriz.h"
#include "interno_cliente.h"
#include "interno_jugador.h"
#include "estructuras_cola.h"

#define PARTIDA_GANADA 1
#define PARTIDA_PERDIDA 0

#define VELOCIDAD_JUEGO 100 // VELOCIDAD_JUEGO podria agregarse como campo de tConfig y modificarse a partir del archivo de texto

int loopPartida(char **matriz, tConfig *conf, SOCKET sockCliente, tCola* colaFantasmas);
int partida(SOCKET sockCliente);
int matrizActualizarPorEstadoDeVidas(char **matriz, tJugador *jug, tCola* colaFantasmas, tConfig *conf, int filaEntrada, int columnaEntrada);
void verRanking(SOCKET sockCliente);

#endif // PARTIDA_H_INCLUDED
