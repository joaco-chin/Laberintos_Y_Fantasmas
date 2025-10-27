#include "principal_archivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigosRet.h"

int inicializarConfConTxt(char *linea, void *buffer, const char *nomArchConf)
{
    FILE *archConfig = fopen(nomArchConf, "rt");
    if (!archConfig)
    {
        puts("No se ha encontrado el archivo de configuracion");
        return ERR_ARCHIVO;
    }

    fgets(linea, TAM_LINEA_CONF, archConfig);
    fclose(archConfig);
    return trozarConfig(linea, buffer);
}

int trozarConfig(char *linea, void *buffer)
{
    tConfig *config = buffer;
    char *ptrAct = strchr(linea, '\n');
    if (!ptrAct)
    {
        return ERR_LINEA_LARGA;
    }
    *ptrAct = '\0';

    ptrAct = strrchr(linea, '|');
    sscanf(ptrAct + 1, "%d", &config->maxVidasExtra);
    *ptrAct = '\0';

    ptrAct = strrchr(linea, '|');
    sscanf(ptrAct + 1, "%d", &config->maxNumPremios);
    *ptrAct = '\0';

    ptrAct = strrchr(linea, '|');
    sscanf(ptrAct + 1, "%d", &config->maxNumFantasmas);
    *ptrAct = '\0';

    ptrAct = strrchr(linea, '|');
    sscanf(ptrAct + 1, "%d", &config->vidasInicio);
    *ptrAct = '\0';

    ptrAct = strrchr(linea, '|');
    sscanf(ptrAct + 1, "%d", &config->col);
    *ptrAct = '\0';

    sscanf(linea, "%d", &config->fil);

    return TODO_OK;
}

void escribirMatrizEnArchivoTxt(char **matriz, const char *nomArch, int filas, int columnas)
{
    FILE *archTxt = fopen(nomArch, "wt");

    if (archTxt == NULL)
    {
        printf("No se ha podido crear el archivo '%s'\n", nomArch);
        return;
    }

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            fputc(matriz[i][j], archTxt);
        }
        fputc('\n', archTxt);
    }

    fclose(archTxt);
}

int matrizInicializarDeArchivoTxt(char **matriz, const char *nomArch, int filas, unsigned tamLinea)
{
    FILE *archTxt = fopen(nomArch, "rt");
    char *linea = malloc(tamLinea);
    char *ptrLinea;
    int i = 0;

    if (archTxt == NULL)
    {
        printf("No se ha encontrado el archivo '%s'\n", nomArch);
        return ERR_ARCHIVO;
    }

    if (linea == NULL)
    {
        puts("Sin memoria");
        fclose(archTxt);
        return SIN_MEM;
    }

    while (fgets(linea, tamLinea, archTxt) && i < filas && ptrLinea != NULL)
    {
        ptrLinea = strchr(linea, '\n');
        if (ptrLinea != NULL)
        {
            *ptrLinea = '\0';
            strcpy(&matriz[i][0], linea);
            i++;
        }
    }

    if (ptrLinea == NULL)
    {
        puts("No se ha encontrado el caracter de fin de cadena");
        return ERR_LINEA_LARGA;
    }

    free(linea);
    fclose(archTxt);
    return TODO_OK;
}
