#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include <winsock2.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int iniciarWinsock();
SOCKET crearSocketServidor();
void procesarPeticion(const char *peticion, char *respuesta);
void correrServidor();

// funcion que va en las funciones que manejan los archivos (base de datos)
int procesarNombreJugador(SOCKET cliente, char *buffer, char *respuesta);

#endif // SERVIDOR_H_INCLUDED
