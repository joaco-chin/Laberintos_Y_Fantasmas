#include "interno_cliente.h"
#include <stdio.h>
#include <string.h>

SOCKET clienteConectarAlServidor()
{
    if (iniciarWinsock() != 0)
    {
        printf("Error al iniciar Winsock.\n");
        return INVALID_SOCKET;
    }

    SOCKET sock = conectarAlServidor(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET)
    {
        printf("Error al conectar al servidor.\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    printf("Conectado al servidor %s:%d\n", SERVER_IP, PORT);

    char nombreUsuario[BUFFER_SIZE];
    char respuesta[BUFFER_SIZE];
    int flag = 1;

    while (flag == 1)
    {
        printf("Ingrese su nombre de usuario:\n> ");
        if (fgets(nombreUsuario, sizeof(nombreUsuario), stdin) == NULL)
        {
            printf("Error al leer el nombre de usuario, intenta de nuevo.\n");
            continue;
        }

        nombreUsuario[strcspn(nombreUsuario, "\n")] = 0;
        eliminarEspaciosAlInicioYFin(nombreUsuario);

        if (strlen(nombreUsuario) == 0)
        {
            printf("El nombre de usuario no puede estar vacio.\n");
            continue;
        }
        flag = 0;
    }

    if (enviarPeticion(sock, nombreUsuario, respuesta) == 0)
        printf("[Servidor]: %s\n", respuesta);
    else
        printf("Error al enviar o recibir datos del servidor.\n");

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