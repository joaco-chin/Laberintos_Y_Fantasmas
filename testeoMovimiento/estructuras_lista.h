#ifndef ESTRUCTURAS_LISTA_H_INCLUDED
#define ESTRUCTURAS_LISTA_H_INCLUDED

#include "estructuras_comun.h"
#include "generico.h"

#define LISTA_VACIA -3
#define LISTA_CON_ELEM -4

typedef tNodo* tLista;

void listaCrear(tLista* pl);
int listaEstaVacia(const tLista* pl);
int listaInsertarAlInicio(tLista* pl, const void* info, unsigned tamInfo);
int listaInsertarOrdenado(tLista* pl, const void* info, unsigned tamInfo, Cmp cmpNodos);
int listaBuscarMenor(const tLista* pl, Cmp cmpNodos);
int listaBuscarPorClave(const tLista* pl, void* clave, Cmp cmpNodos);
int listaRemoverPorPos(tLista* pl, void* info, unsigned tamInfo, int pos);
int listaRemoverInicio(tLista* pl, void* info, unsigned tamInfo);
void listaRecorrer_TEST(const tLista* pl, Accion imp);
void listaVaciar(tLista* pl);

#endif // ESTRUCTURAS_LISTA_H_INCLUDED
