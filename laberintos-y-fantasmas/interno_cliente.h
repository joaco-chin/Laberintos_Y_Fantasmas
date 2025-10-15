#ifndef INTERNO_CLIENTE_H_INCLUDED
#define INTERNO_CLIENTE_H_INCLUDED

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5000
#define BUFFER_SIZE 1024

int iniciarWinsock();
SOCKET conectarAlServidor(const char *servidorIP, int port);
int enviarPeticion(SOCKET sock, const char *peticion, char *respuesta);
void cerrarConexion(SOCKET sock);
SOCKET clienteConectarAlServidor();

// funciones de cadena
void eliminarEspaciosAlInicioYFin(char *str);

#endif // INTERNO_CLIENTE_H_INCLUDED
