#include "peticiones.h"

int procesarNombreJugador(SOCKET cliente, char *buffer, char *respuesta, tArbol *arbolJugadores, FILE *archivoJugadores)
{
    unsigned desplazamiento;
    recv(cliente, buffer, BUFFER_SIZE - 1, 0);
    tNodoArbol **nodoEncontrado = arbolBuscarNodo(arbolJugadores, buffer, sizeof(tJugadorArbol), compararNombresJugadores);

    if (nodoEncontrado != NULL && *nodoEncontrado != NULL)
    {
        // jugador existente
        tJugadorArbol *jugador = (tJugadorArbol *)(*nodoEncontrado)->dato;
        sprintf(respuesta, "jugador existente, bienvenido de nuevo %s", jugador->nombre);
        printf("Jugador existente: %s\n", jugador->nombre);
        desplazamiento = jugador->desplazamiento;
    }
    else
    {
        // nuevo jugador
        tJugadorArbol *nuevoJugador = malloc(sizeof(tJugadorArbol));
        if (nuevoJugador == NULL)
        {
            sprintf(respuesta, "Error al crear nuevo jugador");
            printf("Error al crear nuevo jugador\n");
            return -1;
        }

        fseek(archivoJugadores, 0, SEEK_END);
        unsigned long tamanioArchivo = ftell(archivoJugadores);
        desplazamiento = (unsigned)(tamanioArchivo / sizeof(tJugadorArchivo));

        strcpy(nuevoJugador->nombre, buffer);
        nuevoJugador->id = desplazamiento + 1; // id empieza en 1
        nuevoJugador->desplazamiento = desplazamiento;

        // agregar jugador al archivo de jugadores
        escribirJugadorEnArchivo(nuevoJugador, archivoJugadores);
        sprintf(respuesta, "nuevo jugador, bienvenido %s", nuevoJugador->nombre);
        printf("Nuevo jugador: %s, actualizando base de datos...\n", nuevoJugador->nombre);

        // agregar jugador al arbol de jugadores y actualizar archivo indice
        arbolInsertar(arbolJugadores, nuevoJugador, sizeof(tJugadorArbol), compararNombresJugadores);
        free(nuevoJugador);
        guardarArbolEnArchivo(arbolJugadores, ARCHIVO_INDICE);
    }

    send(cliente, respuesta, strlen(respuesta), 0);

    return desplazamiento;
}

void escribirJugadorEnArchivo(const tJugadorArbol *nuevoJugador, FILE *archivoJugadores)
{
    tJugadorArchivo jugadorArchivo;
    jugadorArchivo.id = nuevoJugador->id;
    strcpy(jugadorArchivo.nombre, nuevoJugador->nombre);
    jugadorArchivo.puntuacion = 0;
    jugadorArchivo.partidasJugadas = 0;
    fwrite(&jugadorArchivo, sizeof(tJugadorArchivo), 1, archivoJugadores);
    fflush(archivoJugadores);
}

void procesarPeticion(const char *peticion, char *respuesta, FILE *archivoJugadores, unsigned desplazamiento, FILE *archivoPartidas)
{
    if (strcmp(peticion, "VER_RANKING") == 0)
    {
        strcpy(respuesta, "\n1 Player1  800\n2 Player2  600\n3 Player3  400\nSIMULACION");
    }
    else if (strncmp(peticion, "GUARDAR_PUNTUACION", 18) == 0)
    {
        actualizarPuntuacionJugador(peticion, archivoJugadores, desplazamiento, archivoPartidas);
        printf("Puntuacion del jugador actualizada y partida registrada.\n");
        strcpy(respuesta, "Partida registrada y puntuacion actualizada correctamente\n");
    }
    else
        strcpy(respuesta, "Comando no reconocido\n");
}

int actualizarPuntuacionJugador(const char *peticion, FILE *archivoJugadores, unsigned desplazamiento, FILE *archivoPartidas)
{
    tJugadorArchivo jugador;

    // leer datos actuales del jugador
    fseek(archivoJugadores, desplazamiento * sizeof(tJugadorArchivo), SEEK_SET);
    fread(&jugador, sizeof(tJugadorArchivo), 1, archivoJugadores);
    fseek(archivoJugadores, 0, SEEK_CUR);

    // extraer nueva puntuacion de la peticion "GUARDAR_PUNTUACION|%d|%d"  -> puntos|cantMovimientos
    unsigned puntos, cantMovimientos;
    char dificultad[MAX_DIFICULTAD];
    sscanf(peticion, "GUARDAR_PUNTUACION|%u|%u|%s", &puntos, &cantMovimientos, dificultad);

    // sumar nueva puntuacion
    jugador.puntuacion += puntos;
    jugador.partidasJugadas += 1;

    // escribir datos actualizados del jugador
    fseek(archivoJugadores, desplazamiento * sizeof(tJugadorArchivo), SEEK_SET);
    fwrite(&jugador, sizeof(tJugadorArchivo), 1, archivoJugadores);
    fflush(archivoJugadores);
    fseek(archivoJugadores, 0, SEEK_CUR);

    escribirPartidaEnArchivo(&jugador, puntos, cantMovimientos, dificultad, archivoPartidas);

    return TODO_OK;
}

void escribirPartidaEnArchivo(tJugadorArchivo *jugador, unsigned puntos, unsigned cantMovimientos, const char *dificultad, FILE *archivoPartidas)
{
    tPartidaArchivo partida;
    fseek(archivoPartidas, 0, SEEK_END);
    unsigned long tamanioArchivo = ftell(archivoPartidas);
    fseek(archivoPartidas, 0, SEEK_CUR);
    partida.idPartida = (unsigned)(tamanioArchivo / sizeof(tPartidaArchivo)) + 1; // idPartida empieza en 1
    partida.idJugador = jugador->id;
    strcpy(partida.nombre, jugador->nombre);
    partida.puntuacionObtenida = puntos;
    partida.cantMovimientos = cantMovimientos;
    strcpy(partida.dificultad, dificultad);
    fwrite(&partida, sizeof(tPartidaArchivo), 1, archivoPartidas);
    fflush(archivoPartidas);
}

// funciones auxiliares de juegadores
void accionMostrarNodoJugadorArbol(void *dato, size_t tamDato, unsigned n, void *params)
{
    // aca se muestra al reves y horizontalmente, los menores a la derecha y los mayores a la izquierda
    tJugadorArbol *jugador = (tJugadorArbol *)dato;
    printf("%*s[%d,%s,%d]\n\n", n * 7, "", jugador->id, jugador->nombre, jugador->desplazamiento);
}

int compararNombresJugadores(const void *nombre, const void *estructuraJugador)
{
    const tJugadorArbol *jugador = (const tJugadorArbol *)estructuraJugador;
    return strcmp((const char *)nombre, jugador->nombre);
}

int compararIDJugadores(const void *jugadorArbolA, const void *jugadorArbolB)
{
    const tJugadorArbol *jugadorA = (const tJugadorArbol *)jugadorArbolA;
    const tJugadorArbol *jugadorB = (const tJugadorArbol *)jugadorArbolB;

    return jugadorA->id - jugadorB->id;
}
