#include "interno_cliente.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "interno_pantalla.h"

SOCKET clienteConectarAlServidor()
{
    if (iniciarWinsock() != 0)
    {
        printw("Error al iniciar Winsock.\n");
        refresh();
        napms(TIEMPO_MENSAJE);
        return INVALID_SOCKET;
    }

    SOCKET sock = conectarAlServidor(SERVER_IP, PORT);
    if(sock == INVALID_SOCKET)
    {
        printw("Error al conectar al servidor.\n");
        refresh();
        napms(TIEMPO_MENSAJE);
        WSACleanup();
        return INVALID_SOCKET;
    }

    printw("Conectado al servidor %s:%d\n", SERVER_IP, PORT);

    char nombreUsuario[BUFFER_SIZE];
    char respuesta[BUFFER_SIZE];
    int flag = 1;

    while(flag == 1)
    {
        printw("Ingrese su nombre de usuario:\n> ");
        refresh();
//        fgets(nombreUsuario, sizeof(nombreUsuario), stdin) == NULL
        if(getnstr(nombreUsuario, sizeof(nombreUsuario) - 1) == ERR)
        {
            printw("Error al leer el nombre de usuario, intenta de nuevo.\n");
            refresh();
            continue;
        }

        nombreUsuario[strcspn(nombreUsuario, "\n")] = 0;
        eliminarEspaciosAlInicioYFin(nombreUsuario);

        if(strlen(nombreUsuario) == 0)
        {
            printw("El nombre de usuario no puede estar vacio.\n");
            refresh();
            continue;
        }
        flag = 0;
    }

    if(enviarPeticion(sock, nombreUsuario, respuesta) == 0)
        printw("[Servidor]: %s\n", respuesta);
    else
        printw("Error al enviar o recibir datos del servidor.\n");

    refresh();
    napms(TIEMPO_MENSAJE * 2);

    return sock;
}

int iniciarWinsock()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

SOCKET conectarAlServidor(const char *servidorIP, int port)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(servidorIP);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

int enviarPeticion(SOCKET sock, const char *peticion, char *respuesta)
{
    if (send(sock, peticion, strlen(peticion), 0) < 0)
    {
        return -1;
    }

    int bytesRecibidos = recv(sock, respuesta, BUFFER_SIZE - 1, 0);
    if (bytesRecibidos <= 0)
    {
        return -1;
    }

    respuesta[bytesRecibidos] = '\0';
    return 0;
}

void cerrarConexion(SOCKET sock)
{
    closesocket(sock);
    WSACleanup();
}

// funciones de cadena
void eliminarEspaciosAlInicioYFin(char *str)
{
    char *inicio = str;
    char *fin = str;

    while (*inicio && isspace((unsigned char)*inicio))
        inicio++;

    if (*inicio == '\0')
    {
        *str = '\0';
        return;
    }

    fin = inicio;
    while (*fin)
        fin++;
    fin--;

    while (fin > inicio && isspace((unsigned char)*fin))
        fin--;
    *(fin + 1) = '\0';

    while (*inicio)
        *str++ = *inicio++;
    *str = '\0';
}
