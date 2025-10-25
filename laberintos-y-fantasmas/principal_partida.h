#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "principal_archivo.h"
#include "interno_matriz.h"
#include "interno_cliente.h"
#include "interno_jugador.h"
#include "estructuras_cola.h"

// Resultados de la partida
#define PARTIDA_GANADA 1
#define PARTIDA_PERDIDA 0

#define VELOCIDAD_JUEGO 1000 // VELOCIDAD_JUEGO podria agregarse como campo de tConfig y modificarse a partir del archivo de texto
#define TIEMPO_MENSAJE 1000

int partida_ejecucion(char **matriz, tConfig *conf, SOCKET sockCliente, tCola* colaFantasmas); // Devuelve el resultado de la partida
int partida_configuracion(SOCKET sockCliente);
int actualizarJuegoPorEstadoDeVidas(char **matriz, tJugador *jug, tCola* colaFantasmas, tConfig *conf, int filaEntrada, int columnaEntrada);
void dibujarPantalla(char **matriz, int cc, int cf, int vidas, int puntos);
void verRanking(SOCKET sockCliente);

#endif // PARTIDA_H_INCLUDED
