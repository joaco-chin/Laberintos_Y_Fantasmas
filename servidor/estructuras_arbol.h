#ifndef ESTRUCTURAS_ARBOL_H_INCLUDED
#define ESTRUCTURAS_ARBOL_H_INCLUDED

#include <stdio.h>

#define MAX_NOMBRE_JUGADOR 50
#define ARCHIVO_INDICE "indice_jugadores.bin" // Archivo binario para persistir el índice

typedef struct NodoArbol {
    char nombreJugador[MAX_NOMBRE_JUGADOR];
    long posicionEnArchivo;
    struct NodoArbol *izquierda;
    struct NodoArbol *derecha;
} NodoArbol;

void insertarJugador(NodoArbol **raiz, const char *nombre, long pos);
NodoArbol* buscarJugador(NodoArbol *raiz, const char *nombre);
void guardarRecursivo(NodoArbol *nodo, FILE *fp);
void guardarIndiceEnArchivo(NodoArbol *raiz, const char *nombreArchivo);
NodoArbol* cargarIndiceDeArchivo(const char *nombreArchivo);
void liberarArbol(NodoArbol *raiz);

#endif // ESTRUCTURAS_ARBOL_H_INCLUDED
