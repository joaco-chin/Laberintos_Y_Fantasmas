#ifndef GENERICO_H_INCLUDED
#define GENERICO_H_INCLUDED

//Macros
#define MIN(a,b)((a)<(b)?(a):(b))
#define MAX(a,b)((a)>(b)?(a):(b))

typedef int(*Cmp)(const void* a, const void* b);
typedef void(*Accion)(const void* a); // Funcion de testing

#endif // GENERICO_H_INCLUDED
