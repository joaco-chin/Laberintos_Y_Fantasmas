#include "estructuras_arbol.h"

void arbolCrear(tArbol *p)
{
    *p = NULL;
}

void arbolVaciar(tArbol *p)
{
    if (*p == NULL)
        return;

    arbolVaciar(&(*p)->izq);
    arbolVaciar(&(*p)->der);
    free((*p)->dato);
    free(*p);
    *p = NULL;
}

int arbolInsertar(tArbol *p, const void *dato, size_t tamDato, Comparar comparar)
{
    int resComp;

    if (*p != NULL)
    {
        resComp = comparar(dato, (*p)->dato);

        if (resComp < 0)
            return arbolInsertar(&(*p)->izq, dato, tamDato, comparar);
        else if (resComp > 0)
            return arbolInsertar(&(*p)->der, dato, tamDato, comparar);
        else // (resComp == 0)
            return DUPLICADO;
    }

    tNodoArbol *nuevo = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if (nuevo == NULL)
        return SIN_MEMORIA;
    nuevo->dato = malloc(tamDato);
    if (nuevo->dato == NULL)
    {
        free(nuevo);
        return SIN_MEMORIA;
    }
    memcpy(nuevo->dato, dato, tamDato);
    nuevo->tamDato = tamDato;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    *p = nuevo;

    return TODO_OK;
}

tNodoArbol **arbolBuscarNodo(tArbol *p, const void *dato, size_t tamDato, Comparar comparar)
{
    int rc;

    if (p == NULL)
        return NULL;

    /* árbol vacío */
    if (*p == NULL)
        return (tNodoArbol **)p;

    rc = comparar(dato, (*p)->dato);

    if (rc < 0)
        return arbolBuscarNodo(&(*p)->izq, dato, tamDato, comparar);
    else if (rc > 0)
        return arbolBuscarNodo(&(*p)->der, dato, tamDato, comparar);

    return (tNodoArbol **)p;
}

int guardarArbolEnArchivo(tArbol *p, const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL)
        return ERROR_ARCHIVO;

    /* pasar el FILE* como params para que la accion pueda escribir */
    arbolRecorrerEnOrden(p, 0, archivo, accionGuardarEnArchivo);
    fclose(archivo);
    return TODO_OK;
}

// estas 3 funciones remplazaría a : cargarIndiceDeArchivo
int cargarArchivoOrdenadoEnArbol(tArbol *p, const char *nombreArchivo, size_t tamDato)
{
    int cantRegistros;
    int r;

    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL)
        return ERROR_ARCHIVO;

    fseek(archivo, 0, SEEK_END);
    cantRegistros = ftell(archivo) / tamDato;

    r = arbolCargarDatosOrdenados(p, archivo, leerDesdeArchivo, 0, cantRegistros - 1, &tamDato);
    fclose(archivo);
    return r;
}

int arbolCargarDatosOrdenados(tArbol *p, void *datos, Leer leer, int li, int ls, void *params)
{
    int m = (li + ls) / 2;
    int r;

    if (li > ls)
        return TODO_OK;

    *p = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if (*p == NULL || ((*p)->tamDato = leer(&(*p)->dato, datos, m, params)) == 0)
    {
        free(*p);
        return SIN_MEMORIA;
    }

    (*p)->izq = NULL;
    (*p)->der = NULL;

    if ((r = arbolCargarDatosOrdenados(&(*p)->izq, datos, leer, li, m - 1, params)) != TODO_OK)
        return r;

    return arbolCargarDatosOrdenados(&(*p)->der, datos, leer, m + 1, ls, params);
}

size_t leerDesdeArchivo(void **dato, void *archivo, unsigned indice, void *params)
{
    size_t tamDato = *((size_t *)params);
    FILE *arch = (FILE *)archivo;

    *dato = malloc(tamDato);
    if (*dato == NULL)
        return 0;

    if (fseek(arch, (long)(indice * tamDato), SEEK_SET) != 0)
    {
        free(*dato);
        *dato = NULL;
        return 0;
    }

    if (fread(*dato, tamDato, 1, arch) != 1)
    {
        free(*dato);
        *dato = NULL;
        return 0;
    }

    return tamDato;
}

// para mostrar el arbol y gurdar en archivo
void arbolRecorrerEnOrden(const tArbol *p, unsigned n, void *params, Accion accion)
{
    if (*p == NULL)
        return;

    arbolRecorrerEnOrden(&(*p)->izq, n + 1, params, accion);
    accion((*p)->dato, (*p)->tamDato, n, params);
    arbolRecorrerEnOrden(&(*p)->der, n + 1, params, accion);
}

// funciones auxiliares de juegadores
void accionGuardarEnArchivo(void *dato, size_t tamDato, unsigned n, void *params)
{
    FILE *archivo = (FILE *)params;
    fwrite(dato, tamDato, 1, archivo);
}
