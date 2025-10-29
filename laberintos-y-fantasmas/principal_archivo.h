#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#define TAM_LINEA_CONF 200
#define TAM_PAL_DIFICULTAD 15

typedef struct
{
    unsigned fil;
    unsigned col;
    unsigned vidasInicio;
    unsigned maxNumFantasmas;
    unsigned maxNumPremios;
    unsigned maxVidasExtra;
    char dificultad[TAM_PAL_DIFICULTAD];
} tConfig;

typedef int (*Trozar)(char *linea, void *buffer, unsigned tamLinea);

int matrizInicializarDeArchivoTxt(char **matriz, const char *nomArch, int filas, unsigned tamLinea);
void escribirMatrizEnArchivoTxt(char **matriz, const char *nomArch, int filas, int columnas);
int inicializarConfConTxt(char *linea, void *buffer, const char *nomArchConf);
int trozarConfig(char *linea, void *buffer);

#endif // ARCHIVO_H_INCLUDED
