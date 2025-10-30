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
        nuevoJugador->id = desplazamiento + 1;
        nuevoJugador->desplazamiento = desplazamiento;

        // agregar jugador al archivo de jugadores
        escribirJugadorEnArchivo(nuevoJugador, archivoJugadores);
        sprintf(respuesta, "nuevo jugador, bienvenido %s", nuevoJugador->nombre);
        printf("Nuevo jugador: %s, actualizando base de datos...\n", nuevoJugador->nombre);

        // agregar jugador al arbol de jugadores y actualizar archivo indice
        arbolInsertar(arbolJugadores, nuevoJugador, sizeof(tJugadorArbol), compararNombresJugadores);
        free(nuevoJugador);
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

void procesarPeticion(const char *peticion, char *respuesta, FILE *archivoJugadores, unsigned desplazamiento, FILE *archivoPartidas, tLista *listaRanking)
{
    // aca se realiza en desencolado (?????s)

    if (strcmp(peticion, "VER_RANKING") == 0)
    {
        mostrarRankingDeJugadores(listaRanking, respuesta);
    }
    else if (strncmp(peticion, "GUARDAR_PUNTUACION", 18) == 0)
    {
        tJugadorArchivo jugadorActualizado = actualizarPuntuacionJugador(peticion, archivoJugadores, desplazamiento, archivoPartidas);
        printf("Partida registrada y puntuacion actualizada para el jugador en desplazamiento %d\n", desplazamiento);
        strcpy(respuesta, "Partida registrada y puntuacion actualizada correctamente\n");
        actualizarListaRanking(listaRanking, &jugadorActualizado, compararPuntuacionJugadores, compararIDJugadores);
    }
    else
        strcpy(respuesta, "Comando no reconocido\n");
}

void mostrarRankingDeJugadores(tLista *listaRanking, char *respuesta)
{
    strcat(respuesta, "---------- Archivo de Jugadores ----------\n");
    strcat(respuesta, "ID|Nombre     |Puntuacion|Partidas Jugadas\n");
    listaRecorrer(listaRanking, accionCopiarNodoListaEnRespuesta, respuesta);
    strcat(respuesta, "------------------------------------------\n\n");
}

tJugadorArchivo actualizarPuntuacionJugador(const char *peticion, FILE *archivoJugadores, unsigned desplazamiento, FILE *archivoPartidas)
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

    return jugador;
}

void escribirPartidaEnArchivo(tJugadorArchivo *jugador, unsigned puntos, unsigned cantMovimientos, const char *dificultad, FILE *archivoPartidas)
{
    tPartidaArchivo partida;
    fseek(archivoPartidas, 0, SEEK_END);
    unsigned long tamanioArchivo = ftell(archivoPartidas);
    fseek(archivoPartidas, 0, SEEK_CUR);
    partida.idPartida = (unsigned)(tamanioArchivo / sizeof(tPartidaArchivo)) + 1;
    partida.idJugador = jugador->id;
    strcpy(partida.nombre, jugador->nombre);
    partida.puntuacionObtenida = puntos;
    partida.cantMovimientos = cantMovimientos;
    strcpy(partida.dificultad, dificultad);
    fwrite(&partida, sizeof(tPartidaArchivo), 1, archivoPartidas);
    fflush(archivoPartidas);
}

void actualizarListaRanking(tLista *listaRanking, const tJugadorArchivo *jugadorActualizado, Cmp cmpPuntuacion, Cmp cmpID)
{
    int pos = listaBuscarPorClave(listaRanking, jugadorActualizado, cmpID);

    if (pos != NO_ENCONTRADO)
    {
        listaRemoverPorPos(listaRanking, NULL, 0, pos);
        listaInsertarOrdenado(listaRanking, jugadorActualizado, sizeof(tJugadorArchivo), cmpPuntuacion);
    }
    else
    {

        int tamanioLista = listaCantidadNodos(listaRanking);
        listaInsertarOrdenado(listaRanking, jugadorActualizado, sizeof(tJugadorArchivo), cmpPuntuacion);
        if (tamanioLista >= 10)
            listaRemoverFinal(listaRanking, NULL, 0);
    }
}

int abrirArchivosDeDatos(FILE **archivoJugadores, FILE **archivoPartidas, FILE **archivoRanking)
{
    *archivoJugadores = fopen(ARCHIVO_JUGADORES, "r+b");
    if (*archivoJugadores == NULL)
    {
        *archivoJugadores = fopen(ARCHIVO_JUGADORES, "w+b");
        if (*archivoJugadores == NULL)
        {
            printf("Error al crear el archivo de jugadores\n");
            return ERROR_ARCHIVO;
        }
    }

    *archivoPartidas = fopen(ARCHIVO_PARTIDAS, "r+b");
    if (*archivoPartidas == NULL)
    {
        *archivoPartidas = fopen(ARCHIVO_PARTIDAS, "w+b");
        if (*archivoPartidas == NULL)
        {
            printf("Error al crear el archivo de partidas\n");
            fclose(*archivoJugadores);
            return ERROR_ARCHIVO;
        }
    }

    *archivoRanking = fopen(ARCHIVO_RANKING, "r+b");
    if (*archivoRanking == NULL)
    {
        *archivoRanking = fopen(ARCHIVO_RANKING, "w+b");
        if (*archivoRanking == NULL)
        {
            printf("Error al crear el archivo de ranking\n");
            fclose(*archivoJugadores);
            fclose(*archivoPartidas);
            return ERROR_ARCHIVO;
        }
    }

    return TODO_OK;
}

void cerrarArchivosDeDatos(FILE *archivoJugadores, FILE *archivoPartidas, FILE *archivoRanking)
{
    fclose(archivoJugadores);
    fclose(archivoPartidas);
    fclose(archivoRanking);
}

int archivoRankingAlista(tLista *listaRanking, FILE *archivoRanking)
{
    tJugadorArchivo jugador;
    fseek(archivoRanking, 0, SEEK_SET);
    while (fread(&jugador, sizeof(tJugadorArchivo), 1, archivoRanking))
    {
        listaInsertarAlFinal(listaRanking, &jugador, sizeof(tJugadorArchivo));
    }
    return TODO_OK;
}

int listaRankingAarchivo(tLista *listaRanking, FILE *archivoRanking)
{
    tNodo *actual = *listaRanking;
    fseek(archivoRanking, 0, SEEK_SET);
    while (actual != NULL)
    {
        fwrite(actual->info, sizeof(tJugadorArchivo), 1, archivoRanking);
        actual = actual->sig;
    }
    fflush(archivoRanking);
    return TODO_OK;
}

// funciones auxiliares de juegadores
void accionMostrarNodoJugadorArbol(void *dato, size_t tamDato, unsigned n, void *params)
{
    // aca se muestra al reves y horizontalmente, los menores a la derecha y los mayores a la izquierda
    tJugadorArbol *jugador = (tJugadorArbol *)dato;
    printf("%*s[%d,%s,%d]\n\n", n * 7, "", jugador->id, jugador->nombre, jugador->desplazamiento);
}

void accionMostrarNodoJugadorLista(const void *a)
{
    const tJugadorArchivo *jugador = a;
    printf("%s|%d|%d\n", jugador->nombre, jugador->puntuacion, jugador->partidasJugadas);
}

void accionCopiarNodoListaEnRespuesta(void *destino, const void *origen)
{
    char *respuesta = destino;
    const tJugadorArchivo *jugador = origen;
    char mensaje[200];
    sprintf(mensaje, "%2d|%-11s|%10d|%16d\n", jugador->id, jugador->nombre, jugador->puntuacion, jugador->partidasJugadas);
    strcat(respuesta, mensaje);
}

int compararNombresJugadores(const void *nombre, const void *estructuraJugador)
{
    const tJugadorArbol *jugador = (const tJugadorArbol *)estructuraJugador;
    return strcmp((const char *)nombre, jugador->nombre);
}

int compararIDJugadores(const void *jugadorA, const void *jugadorB)
{
    const tJugadorArchivo *j1 = (const tJugadorArchivo *)jugadorA;
    const tJugadorArchivo *j2 = (const tJugadorArchivo *)jugadorB;

    return j1->id - j2->id;
}

int compararPuntuacionJugadores(const void *jugadorA, const void *jugadorB)
{
    const tJugadorArchivo *j1 = (const tJugadorArchivo *)jugadorA;
    const tJugadorArchivo *j2 = (const tJugadorArchivo *)jugadorB;

    return j1->puntuacion - j2->puntuacion;
}
