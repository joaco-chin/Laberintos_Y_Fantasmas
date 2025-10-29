#ifndef ESTRUCTURAS_LISTA_H_INCLUDED
#define ESTRUCTURAS_LISTA_H_INCLUDED

typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo* sig;
}tNodo;

#include "..\comun\generico.h"

#define LISTA_VACIA -3
#define LISTA_CON_ELEM -4

typedef tNodo* tLista;

void listaCrear(tLista* pl);
int listaEstaVacia(const tLista* pl);
int listaInsertarAlInicio(tLista* pl, const void* info, unsigned tamInfo);
int listaInsertarOrdenado(tLista* pl, const void* info, unsigned tamInfo, Cmp cmpNodos);
int listaBuscarMenor(const tLista* pl, Cmp cmpNodos);
int listaBuscarPorClave(const tLista* pl, void* clave, Cmp cmpNodos);
int listaRemoverInicio(tLista* pl, void* info, unsigned tamInfo);
int listaRemoverPorPos(tLista* pl, void* info, unsigned tamInfo, int pos);
int listaVerPos(const tLista* pl, void* info, unsigned tamInfo, int pos);
//void listaRecorrer(const tLista* pl, Imp imp);
void listaRecorrer(const tLista* pl, Copy cpy, char* str);
void listaVaciar(tLista* pl);

#endif // ESTRUCTURAS_LISTA_H_INCLUDED
