#ifndef ESTRUCTURAS_COMUN_H_INCLUDED
#define ESTRUCTURAS_COMUN_H_INCLUDED

typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo* sig;
}tNodo;

#endif // ESTRUCTURAS_COMUN_H_INCLUDED
