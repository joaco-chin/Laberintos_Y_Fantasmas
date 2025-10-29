#ifndef ESTRUCTURAS_ARBOL_H_INCLUDED
#define ESTRUCTURAS_ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 0
#define SIN_MEMORIA 1
#define DUPLICADO 2
#define ERROR_ARCHIVO 3

typedef struct sNodoArbol
{
    void *dato;
    size_t tamDato;
    struct sNodoArbol *izq, *der;
} tNodoArbol;

typedef tNodoArbol *tArbol;

// punteros a funciones
typedef int (*Comparar)(const void *, const void *);
typedef void (*Accion)(void *, size_t, unsigned, void *);
typedef size_t (*Leer)(void **, void *, unsigned, void *);

void arbolCrear(tArbol *p);
void arbolVaciar(tArbol *p);
int arbolInsertar(tArbol *p, const void *dato, size_t tamDato, Comparar comparar);
tNodoArbol **arbolBuscarNodo(tArbol *p, const void *dato, size_t tamDato, Comparar comparar);

// funcion para guardar el arbol en un archivo ordenado
void guardarRecursivo(tArbol *p, FILE *archivo);
int guardarArbolEnArchivo(tArbol *p, const char *nombreArchivo);

// estas 3 funciones remplazaría a : cargarIndiceDeArchivo
int cargarArchivoOrdenadoEnArbol(tArbol *p, const char *nombreArchivo, size_t tamDato);
int arbolCargarDatosOrdenados(tArbol *p, void *datos, Leer leer, int li, int ls, void *params);
size_t leerDesdeArchivo(void **dato, void *archivo, unsigned indice, void *params);

// para mostrar el arbol y gurdar en archivo
void arbolRecorrerEnOrden(const tArbol *p, unsigned n, void *params, Accion accion);

void accionGuardarEnArchivo(void *dato, size_t tamDato, unsigned n, void *params);

#endif // ESTRUCTURAS_ARBOL_H_INCLUDED
