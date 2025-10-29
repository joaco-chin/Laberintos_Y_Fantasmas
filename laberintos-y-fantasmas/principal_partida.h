#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "principal_archivo.h"
#include "interno_matriz.h"
#include "interno_cliente.h"
#include "interno_jugador.h"
#include "interno_laberinto.h"
#include "estructuras_cola.h"

// Resultados de la partida
#define PARTIDA_GANADA 1
#define PARTIDA_PERDIDA 0
// Tiempos de los eventos durante la ejecución
#define TIEMPO_INPUT 100
#define TIEMPO_FRAME 100
#define TIEMPO_MENSAJE 1000
// Bonificaciones por dificultad
#define BONIFICACION_FACIL 1
#define BONIFICACION_NORMAL 5
#define BONIFICACION_DIFICIL 25
#define BONIFICACION_PESADILLA 125

int actualizarPartidaPorEstadoDeVidas(char **matriz, tJugador *jug, tCola* colaFantasmas, tConfig *conf, int filaEntrada, int columnaEntrada);
int procesarAccionDeJugador(char **matriz, int cf, int cc, tJugador *jug, tCola *registro);
int procesarEventosDePartida(char **matriz, tConfig *conf, tJugador *jug, tCola *colaDeFantasmas, tCola *movimientos, tPosicion entradaYSalida[]);
int determinarBonificacion(const char *dif);
void dibujarInicioPantalla(char **matriz, int cc, int cf);
void dibujarPantalla(char **matriz, int cc, int cf, const char* dificultad, int vidas, int puntos);

int ejecucionPartida(char **matriz, tConfig *conf, SOCKET sockCliente, tCola* colaFantasmas, tPosicion entradaYSalida[]); // Devuelve el resultado de la partida
int configuracionPartida(SOCKET sockCliente);
void verRanking(SOCKET sockCliente);

#endif // PARTIDA_H_INCLUDED
