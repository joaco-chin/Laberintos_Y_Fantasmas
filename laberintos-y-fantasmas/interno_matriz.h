#ifndef MATRIZ_H_INCLUDED
#define MATRIZ_H_INCLUDED

void **matrizCrear(unsigned tamElem, int filas, int columnas);
void inicializarMatrizCaracter(char **matriz, int filas, int columnas, char caracter);
void matrizMostrar(char **matriz, int filas, int columnas);
void matrizBuscar(char **matriz, char objetivo, int *fila, int *columna, int cf, int cc);
void matrizRemplazarCaracterEnPosicion(char **matriz, char nuevoCaracter, int fila, int columna, int cf, int cc);
void matrizDestruir(void **matriz, int filas);

#endif // MATRIZ_H_INCLUDED
