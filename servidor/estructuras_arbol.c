#include "estructuras_arbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodoArbol* crearNodo(const char *nombre, long pos) {
    NodoArbol *nuevoNodo = (NodoArbol*)malloc(sizeof(NodoArbol));
    if (nuevoNodo == NULL) {
        perror("Error al insertar nuevo ingreso en el indice\n");
        exit(EXIT_FAILURE);
    }
    strncpy(nuevoNodo->nombreJugador, nombre, MAX_NOMBRE_JUGADOR - 1);
    nuevoNodo->nombreJugador[MAX_NOMBRE_JUGADOR - 1] = '\0';
    nuevoNodo->posicionEnArchivo = pos;
    nuevoNodo->izquierda = NULL;
    nuevoNodo->derecha = NULL;

    return nuevoNodo;
}

void insertarJugador(NodoArbol **raiz, const char *nombre, long pos) {
    if (*raiz == NULL) {
        *raiz = crearNodo(nombre, pos);
    } else {
        int cmp = strcmp(nombre, (*raiz)->nombreJugador);
        if (cmp < 0) {
            insertarJugador(&((*raiz)->izquierda), nombre, pos);
        } else if (cmp > 0) {
            insertarJugador(&((*raiz)->derecha), nombre, pos);
        }
    }
}

NodoArbol* buscarJugador(NodoArbol *raiz, const char *nombre) {
    if (raiz == NULL) {
        return NULL;
    }

    int cmp = strcmp(nombre, raiz->nombreJugador);

    if (cmp == 0) {
        return raiz;
    } else if (cmp < 0) {
        return buscarJugador(raiz->izquierda, nombre);
    } else {
        return buscarJugador(raiz->derecha, nombre);
    }
}

void guardarRecursivo(NodoArbol *nodo, FILE *fp) {
    if (nodo != NULL) {
        fwrite(nodo->nombreJugador, sizeof(char), MAX_NOMBRE_JUGADOR, fp);
        fwrite(&(nodo->posicionEnArchivo), sizeof(long), 1, fp);
        guardarRecursivo(nodo->izquierda, fp);
        guardarRecursivo(nodo->derecha, fp);
    }
}

void guardarIndiceEnArchivo(NodoArbol *raiz, const char *nombreArchivo) {
    FILE *fp = fopen(nombreArchivo, "wb");
    if (fp == NULL) {
        perror("Error al abrir archivo de indice para escritura\n");
        return;
    }
    guardarRecursivo(raiz, fp);
    fclose(fp);
}

NodoArbol* cargarIndiceDeArchivo(const char *nombreArchivo) {
    FILE *fp = fopen(nombreArchivo, "rb");
    if (fp == NULL) {
        // No se encontró archivo de índice. Se creará uno nuevo al guardar.
        return NULL;
    }

    NodoArbol *raiz = NULL;
    char nombreBuffer[MAX_NOMBRE_JUGADOR];
    long posBuffer;

    // Leemos el archivo registro por registro
    while (fread(nombreBuffer, sizeof(char), MAX_NOMBRE_JUGADOR, fp) == MAX_NOMBRE_JUGADOR) {
        if (fread(&posBuffer, sizeof(long), 1, fp) != 1) {
            fprintf(stderr, "Error por archivo de índice corrupto.\n");
            break;
        }
        insertarJugador(&raiz, nombreBuffer, posBuffer);
    }

    fclose(fp);
    return raiz;
}

void liberarArbol(NodoArbol *raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izquierda);
        liberarArbol(raiz->derecha);
        free(raiz);
    }
}

