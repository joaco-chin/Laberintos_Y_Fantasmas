#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void correrServidor()
{
    if (iniciarWinsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    SOCKET servidor = crearSocketServidor();
    if (servidor == INVALID_SOCKET)
    {
        printf("Error al crear el socket del servidor\n");
        WSACleanup();
        return;
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    SOCKET cliente = accept(servidor, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (cliente == INVALID_SOCKET)
    {
        printf("Error en accept\n");
        closesocket(servidor);
        WSACleanup();
        return;
    }

    char buffer[BUFFER_SIZE];
    char respuesta[BUFFER_SIZE];
    int bytesRecibidos;

    printf("Cliente conectado\n");
    procesarNombreJugador(cliente, buffer, respuesta);

    while ((bytesRecibidos = recv(cliente, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytesRecibidos] = '\0';
        printf("Peticion recibida: %s\n", buffer);

        procesarPeticion(buffer, respuesta);

        send(cliente, respuesta, strlen(respuesta), 0);
    }

    printf("Cliente desconectado\n");
    closesocket(cliente);
    closesocket(servidor);
    WSACleanup();
}

void procesarPeticion(const char *peticion, char *respuesta)
{
    if (strcmp(peticion, "VER_RANKING") == 0)
    {
        strcpy(respuesta, "\n1 Player1  800\n2 Player2  600\n3 Player3  400\nSIMULACION");
    }
    else if (strncmp(peticion, "GUARDAR_PUNTUACION", 18) == 0) // compara solo los primeros 18 caracteres
    {
        strcpy(respuesta, "Puntuacion guardada correctamente SIMULACION");
    }
    else
        strcpy(respuesta, "Comando no reconocido\n");
}

SOCKET crearSocketServidor()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        perror("Error en bind");
        closesocket(s);
        exit(EXIT_FAILURE);
    }

    if (listen(s, SOMAXCONN) == SOCKET_ERROR)
    {
        perror("Error en listen");
        closesocket(s);
        exit(EXIT_FAILURE);
    }

    return s;
}

int iniciarWinsock()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

//funcion que va en las funciones que manejan los archivos (base de datos)
int procesarNombreJugador(SOCKET cliente, char *buffer, char *respuesta)
{
    recv(cliente, buffer, BUFFER_SIZE - 1, 0);
    // si existe: "jugador existente"
    sprintf(respuesta, "jugador existente, bienvenido de nuevo %s SIMULACION", buffer);
    printf("Nombre del jugador: %s\n", buffer);
    
    // si no existe: "nuevo jugador": se actualiza la base de datos
    // strcpy(respuesta, "nuevo jugador, bienvenido");
    // printf("Nombre del nuevo jugador: %s, actualizando base de datos...\n", buffer);

    send(cliente, respuesta, strlen(respuesta), 0);

    return 0;
}