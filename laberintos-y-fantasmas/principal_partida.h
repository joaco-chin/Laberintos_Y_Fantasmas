#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "principal_archivo.h"
#include "interno_matriz.h"
#include "interno_cliente.h"
#include "interno_jugador.h"
#include "interno_laberinto.h"
#include "interno_pantalla.h"
#include "estructuras_cola.h"

// Resultados de la partida
#define PARTIDA_GANADA 1
#define PARTIDA_PERDIDA 0
// Tiempos de los eventos durante la ejecución
#define TIEMPO_INPUT 100
// Bonificaciones por dificultad
#define BONIFICACION_FACIL 1
#define BONIFICACION_NORMAL 5
#define BONIFICACION_DIFICIL 25
#define BONIFICACION_PESADILLA 125
//MACROS operaciones
#define ES_MOVIMIENTO(x)((x) == KEY_UP || (x) == KEY_DOWN || (x) == KEY_LEFT || (x) == KEY_RIGHT ? 1 : 0)

int actualizarPartidaPorEstadoDeVidas(char **matriz, tJugador *jug, tLista* fantasmas, tConfig *conf, int filaEntrada, int columnaEntrada);
int procesarAccionDeJugador(char **matriz, int cf, int cc, tJugador *jug, tLista *registro, tCola* movimientos, int altoStdscr, int anchoStdscr);
void desencolarMovimientosPartida(char** matriz, int cf, int cc, tCola* movimientos, tLista* listaFantasmas);
int determinarBonificacion(const char *dif);
void dibujarInicioPantalla(char **matriz, int cc, int cf);
void dibujarPantalla(char **matriz, int cc, int cf, const char* dificultad, int vidas, int puntos);

int ejecucionPartida(char **matriz, tConfig *conf, SOCKET sockCliente, tLista* fantasmas, tPosicion entradaYSalida[], int altoStdscr, int anchoStdscr);
// Devuelve el resultado de la partida
int configuracionPartida(SOCKET sockCliente, int altoStdscr, int anchoStdscr);
void verRanking(SOCKET sockCliente);

#endif // PARTIDA_H_INCLUDED
