#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estructuras_arbol.h"
#include "peticiones.h"

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

    // INDICE A ARBOL
    tArbol arbolJugadores;
    arbolCrear(&arbolJugadores);
    if (cargarArchivoOrdenadoEnArbol(&arbolJugadores, ARCHIVO_INDICE, sizeof(tJugadorArbol)) == ERROR_ARCHIVO)
    {
        FILE *archivo = fopen(ARCHIVO_INDICE, "wb");
        if (archivo == NULL)
        {
            printf("Error al crear el archivo de indice de jugadores\n");
            closesocket(cliente);
            closesocket(servidor);
            WSACleanup();
            return;
        }
        printf("Archivo de indice de jugadores no encontrado. Se ha creado uno nuevo: %s.\n", ARCHIVO_INDICE);
    }

    printf("Arbol de jugadores cargado desde el archivo de indice:\n");
    arbolRecorrerEnOrden(&arbolJugadores, 0, NULL, accionMostrarNodoJugadorArbol);

    // ABRIR ARCHIVO JUGADORES
    // Abrir en modo lectura/actualizacion binaria. Si no existe, crearlo en modo w+b.
    FILE *archivoJugadores = fopen(ARCHIVO_JUGADORES, "r+b");
    if (archivoJugadores == NULL)
    {
        archivoJugadores = fopen(ARCHIVO_JUGADORES, "w+b");
        if (archivoJugadores == NULL)
        {
            printf("Error al crear el archivo de jugadores\n");
            closesocket(cliente);
            closesocket(servidor);
            WSACleanup();
            return;
        }
    }

    // ABRIR ARCHIVO PARTIDAS
    FILE *archivoPartidas = fopen(ARCHIVO_PARTIDAS, "r+b");
    if (archivoPartidas == NULL)
    {
        archivoPartidas = fopen(ARCHIVO_PARTIDAS, "w+b");
        if (archivoPartidas == NULL)
        {
            printf("Error al crear el archivo de partidas\n");
            closesocket(cliente);
            closesocket(servidor);
            WSACleanup();
            return;
        }
    }

    printf("Cliente conectado\n");
    int desplazamiento = procesarNombreJugador(cliente, buffer, respuesta, &arbolJugadores, archivoJugadores);

    while ((bytesRecibidos = recv(cliente, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytesRecibidos] = '\0';
        printf("Peticion recibida: %s\n", buffer);

        procesarPeticion(buffer, respuesta, archivoJugadores, desplazamiento, archivoPartidas);

        send(cliente, respuesta, strlen(respuesta), 0);
    }

    printf("Cliente desconectado\n");

    arbolVaciar(&arbolJugadores);
    fclose(archivoJugadores);
    fclose(archivoPartidas);

    closesocket(cliente);
    closesocket(servidor);
    WSACleanup();
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
