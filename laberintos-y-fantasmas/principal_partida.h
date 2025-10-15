#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "principal_archivo.h"
#include "interno_matriz.h"
#include "interno_cliente.h"

#define PARTIDA_GANADA 1
#define PARTIDA_PERDIDA 0

int loopPartida(char **matriz, tConfig *conf, SOCKET sockCliente);
int partida(SOCKET sockCliente);
void verRanking(SOCKET sockCliente);

#endif // PARTIDA_H_INCLUDED
