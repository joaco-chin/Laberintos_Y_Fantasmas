#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include <winsock2.h>

#define PORT 5000
#define BUFFER_SIZE 1024
#define ARCHIVO_INDICE "indice_jugadores.bin"

int iniciarWinsock();
SOCKET crearSocketServidor();
void correrServidor();

#endif // SERVIDOR_H_INCLUDED
