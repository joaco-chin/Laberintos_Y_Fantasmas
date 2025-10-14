#include "interno_laberinto.h"

void generarLaberintoAleatorio(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios, unsigned *vidasExtra)
{
    tPosicion posEnt, posSal;
    int maxBloquesPorPared = 2;

    inicializarMatrizCaracter(matriz, filas, columnas, CAMINO);
    generarEntradaYSalida(matriz, filas, columnas, &posEnt, &posSal);
    evitarObstrucciones(matriz, filas, columnas, posEnt, posSal, maxBloquesPorPared);
    generarParedesInternas(matriz, filas, columnas, maxBloquesPorPared);
    generarParedesLimite(matriz, filas, columnas, posEnt, posSal);
    colocarCaracteresEspeciales(matriz, filas, columnas, fantasmas, premios, vidasExtra);
    actualizarCaracterAlrededorDePosicion(matriz, filas, columnas, posEnt, PARED_RANGO_ENTRADA, CAMINO, maxBloquesPorPared + 1);
}

void inicializarMatrizCaracter(char **matriz, int filas, int columnas, char caracter)
{
    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            matriz[i][j] = caracter;
}

void generarParedesLimite(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal)
{
    // Paredes verticales (izquierda y derecha)
    for (int i = 0; i < filas; i++)
    {
        // Pared izquierda - no pisar entrada ni salida
        if (!(i == posEnt.fila && 0 == posEnt.columna) && !(i == posSal.fila && 0 == posSal.columna))
        {
            matriz[i][0] = PARED;
        }

        // Pared derecha - no pisar entrada ni salida
        if (!(i == posEnt.fila && (columnas - 1) == posEnt.columna) && !(i == posSal.fila && (columnas - 1) == posSal.columna))
        {
            matriz[i][columnas - 1] = PARED;
        }
    }

    // Paredes horizontales (arriba y abajo)
    for (int j = 1; j < columnas - 1; j++)
    {
        // Pared superior - no pisar entrada ni salida
        if (!(0 == posEnt.fila && j == posEnt.columna) && !(0 == posSal.fila && j == posSal.columna))
        {
            matriz[0][j] = PARED;
        }

        // Pared inferior - no pisar entrada ni salida
        if (!((filas - 1) == posEnt.fila && j == posEnt.columna) && !((filas - 1) == posSal.fila && j == posSal.columna))
        {
            matriz[filas - 1][j] = PARED;
        }
    }
}

void generarParedesInternas(char **matriz, int filas, int columnas, int maxBloquesPorPared)
{
    int ordenDirecciones[] = {0, 1, 2, 3};

    tLista listaPosLibres;
    listaCrear(&listaPosLibres);
    int posLibres = llenarListaPosicionesLibresParedes(matriz, filas, columnas, &listaPosLibres);

    while (posLibres > 0)
    {
        tPosicion posActual = elegirYEliminarPosicionLista(&listaPosLibres, &posLibres);

        if (matriz[posActual.fila][posActual.columna] == ' ' && !hayCaracterAlrededorDePosicion(matriz, filas, columnas, posActual.fila, posActual.columna, PARED))
        {
            matriz[posActual.fila][posActual.columna] = PARED_TEMPORAL;
            barajarOrdenDirecciones(ordenDirecciones, 4);
            int direccionesUsadas = 0;
            int direccionesAUtilizar = 3 + (rand() % 2); // entre 3 y 4 direcciones

            for (int d = 0; d < CANT_MAX_DIREC && direccionesUsadas < direccionesAUtilizar; d++)
            {
                int dirElegida = ordenDirecciones[d];

                colocarParedTemporalEnPosicion(matriz, filas, columnas, posActual, dirElegida, maxBloquesPorPared);

                direccionesUsadas++;
            }
        }

        actualizarCaracterAlrededorDePosicion(matriz, filas, columnas, posActual, PARED_TEMPORAL, PARED, maxBloquesPorPared);
    }
    listaVaciarREVISAR(&listaPosLibres);
}

void barajarOrdenDirecciones(int *ordenDirecciones, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int j = i + rand() % (n - i);
        int temp = ordenDirecciones[i];
        ordenDirecciones[i] = ordenDirecciones[j];
        ordenDirecciones[j] = temp;
    }
}

int hayCaracterAlrededorDePosicion(char **matriz, int filas, int columnas, int i, int j, char objetivo)
{
    for (int k = i - 1; k <= i + 1; k++)
    {
        for (int l = j - 1; l <= j + 1; l++)
        {
            if (k >= 0 && k < filas && l >= 0 && l < columnas && (k != i || l != j))
                if (matriz[k][l] == objetivo)
                    return VERDADERO;
        }
    }
    return FALSO;
}

void colocarParedTemporalEnPosicion(char **matriz, int filas, int columnas, tPosicion posActual, int dirElegida, int cantBloquesMax)
{
    tPosicion dir[] = {
        {-1, 0}, // arriba
        {0, 1},  // derecha
        {1, 0},  // abajo
        {0, -1}  // izquierda
    };

    tPosicion pos;
    int k = 1;
    int continuar = VERDADERO;
    while (k <= cantBloquesMax && continuar)
    {
        pos.fila = posActual.fila + k * dir[dirElegida].fila;
        pos.columna = posActual.columna + k * dir[dirElegida].columna;

        if (pos.fila > 0 && pos.fila < filas - 1 && pos.columna > 0 && pos.columna < columnas - 1 && !hayCaracterAlrededorDePosicion(matriz, filas, columnas, pos.fila, pos.columna, PARED))
            matriz[pos.fila][pos.columna] = PARED_TEMPORAL;
        else
            continuar = FALSO;
        k++;
    }
}

void generarEntradaYSalida(char **matriz, int filas, int columnas, tPosicion *posEnt, tPosicion *posSal)
{
    posEnt->fila = rand() % (filas);

    if (posEnt->fila == 0 || posEnt->fila == filas - 1)
        posEnt->columna = rand() % (columnas - 2) + 1;
    else
        posEnt->columna = (rand() % 2) == 0 ? 0 : columnas - 1;

    posSal->fila = filas - posEnt->fila - 1;
    posSal->columna = columnas - posEnt->columna - 1;
    matriz[posEnt->fila][posEnt->columna] = ENTRADA;
    matriz[posSal->fila][posSal->columna] = SALIDA;
}

void evitarObstrucciones(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal, int maxBloquesPorPared)
{
    // evita obstrucciones en las esquinas de la matriz
    for (int j = 0; j <= maxBloquesPorPared; j++)
    {
        matriz[0][j] = PARED;
        matriz[0][columnas - 1 - j] = PARED;
        matriz[filas - 1][j] = PARED;
        matriz[filas - 1][columnas - 1 - j] = PARED;
    }

    colocarCaracterEnEsquinasDePosicion(matriz, filas, columnas, posEnt, PARED);
    colocarCaracterEnEsquinasDePosicion(matriz, filas, columnas, posSal, PARED);
    actualizarCaracterAlrededorDePosicion(matriz, filas, columnas, posEnt, CAMINO, PARED_RANGO_ENTRADA, maxBloquesPorPared + 1);
}

void actualizarCaracterAlrededorDePosicion(char **matriz, int filas, int columnas, tPosicion pos, char car, char nuevoCar, int rango)
{
    int fs = pos.fila - rango >= 0 ? pos.fila - rango : 0;
    int fi = pos.fila + rango < filas ? pos.fila + rango : filas - 1;
    int cs = pos.columna - rango >= 0 ? pos.columna - rango : 0;
    int ci = pos.columna + rango < columnas ? pos.columna + rango : columnas - 1;

    for (int i = fs; i <= fi; i++)
        for (int j = cs; j <= ci; j++)
            if (matriz[i][j] == car)
                matriz[i][j] = nuevoCar;
}

void colocarCaracterEnEsquinasDePosicion(char **matriz, int filas, int columnas, tPosicion pos, char caracter)
{
    int fs = pos.fila - 1 >= 0 ? pos.fila - 1 : 0;
    int fi = pos.fila + 1 < filas ? pos.fila + 1 : filas - 1;
    int cs = pos.columna - 1 >= 0 ? pos.columna - 1 : 0;
    int ci = pos.columna + 1 < columnas ? pos.columna + 1 : columnas - 1;

    for (int i = fs; i <= fi; i += (fi - fs))     // solo las filas de los extremos
        for (int j = cs; j <= ci; j += (ci - cs)) // solo las columnas de los extremos
            if (matriz[i][j] == ' ')
                matriz[i][j] = caracter;
}

void colocarCaracteresEspeciales(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios, unsigned *vidasExtra)
{
    unsigned cantFantasmas = colocarCaracteresEnPosicionesAleatorias(matriz, filas, columnas, FANTASMA, *fantasmas);
    if (cantFantasmas != *fantasmas)
    {
        printf("Se generaron %d fantasmas en lugar de %d\n", cantFantasmas, *fantasmas);
        *fantasmas = cantFantasmas;
    }
    unsigned cantVidasExtra = colocarCaracteresEnPosicionesAleatorias(matriz, filas, columnas, VIDA_EXTRA, *vidasExtra);
    if (cantVidasExtra != *vidasExtra)
    {
        printf("Se generaron %d vidas extra en lugar de %d\n", cantVidasExtra, *vidasExtra);
        *vidasExtra = cantVidasExtra;
    }
    unsigned cantPremios = colocarCaracteresEnPosicionesAleatorias(matriz, filas, columnas, PREMIO, *premios);
    if (cantPremios != *premios)
    {
        printf("Se generaron %d premios en lugar de %d\n", cantPremios, *premios);
        *premios = cantPremios;
    }
}

int colocarCaracteresEnPosicionesAleatorias(char **matriz, int filas, int columnas, char caracter, int cantidadCar)
{
    int k = 0;
    tLista listaPosLibres;
    listaCrear(&listaPosLibres);
    int cantLibres = llenarListaPosicionesLibres(matriz, filas, columnas, &listaPosLibres);
    if (cantLibres == 0)
    {
        listaVaciarREVISAR(&listaPosLibres);
        return k;
    }

    for (k = 0; k < cantidadCar && cantLibres > 0; k++)
    {
        tPosicion p = elegirYEliminarPosicionLista(&listaPosLibres, &cantLibres);
        matriz[p.fila][p.columna] = caracter;
    }
    listaVaciarREVISAR(&listaPosLibres);
    return k;
}

int llenarListaPosicionesLibres(char **matriz, int filas, int columnas, tLista *lista)
{
    tPosicion pos;
    int count = 0;
    for (int i = 1; i < filas - 1; i++)
    {
        for (int j = 1; j < columnas - 1; j++)
        {
            if (matriz[i][j] == CAMINO)
            {
                pos.fila = i;
                pos.columna = j;
                if (listaPonerAlFinalREVISAR(lista, &pos, sizeof(tPosicion)) == TODO_OK)
                    count++;
            }
        }
    }
    return count;
}

int llenarListaPosicionesLibresParedes(char **matriz, int filas, int columnas, tLista *lista)
{
    tPosicion pos;
    int count = 0;

    for (int i = 2; i < filas - 2; i += 2)
    {
        for (int j = 2; j < columnas - 2; j += 2)
        {
            if (matriz[i][j] == CAMINO)
            {
                pos.fila = i;
                pos.columna = j;
                if (listaPonerAlFinalREVISAR(lista, &pos, sizeof(tPosicion)) == TODO_OK)
                    count++;
            }
        }
    }
    return count;
}

tPosicion elegirYEliminarPosicionLista(tLista *lista, int *cantidad)
{
    int pos = rand() % (*cantidad) + 1; // rango de 1 a cantidad
    tPosicion posXY;
    listaObtenerDatoPorPosicionREVISAR(lista, pos, &posXY, sizeof(tPosicion));
    listaEliminarNodoPorPosicionREVISAR(lista, pos, &posXY, sizeof(tPosicion));
    (*cantidad)--;

    return posXY;
}

// funciones de lista REVISAR, función para poner en esctructuras_lista
int listaObtenerDatoPorPosicionREVISAR(tLista *lista, int posicion, void *dato, size_t tamDato)
{
    int i = 0;

    if (posicion < 1)
        return -1; // POSICION_INVALIDA;

    while (*lista != NULL && i < posicion - 1)
    {
        lista = &(*lista)->sig;
        i++;
    }

    if (*lista == NULL)
        return LISTA_VACIA;
    memcpy(dato, (*lista)->info, MIN((*lista)->tamInfo, tamDato));
    return TODO_OK;
}

int listaEliminarNodoPorPosicionREVISAR(tLista *lista, int posicion, void *dato, size_t tamDato)
{
    if (*lista == NULL)
        return LISTA_VACIA;

    if (posicion < 1)
        return -1; // POSICION_INVALIDA;

    // caso especial si es el primero
    if (posicion == 1)
    {
        memcpy(dato, (*lista)->info, MIN((*lista)->tamInfo, tamDato));
        tNodo *aux = *lista;
        *lista = aux->sig;
        free(aux->info);
        free(aux);
        return TODO_OK;
    }

    int i = 1;
    while (*lista != NULL && i < posicion - 1)
    {
        lista = &(*lista)->sig;
        i++;
    }

    if (*lista == NULL || (*lista)->sig == NULL)
        return -1; // POSICION_INVALIDA;

    memcpy(dato, (*lista)->sig->info, MIN((*lista)->sig->tamInfo, tamDato));

    tNodo *aux = (*lista)->sig;
    (*lista)->sig = aux->sig;
    free(aux->info);
    free(aux);
    return TODO_OK;
}

int listaPonerAlFinalREVISAR(tLista *pl, const void *dato, size_t tamDato)
{
    tNodo *nuevo = (tNodo *)malloc(sizeof(tNodo));
    if (nuevo == NULL)
        return SIN_MEM;
    nuevo->info = malloc(tamDato);
    if (nuevo->info == NULL)
    {
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->info, dato, tamDato);
    nuevo->tamInfo = tamDato;
    nuevo->sig = NULL;

    while (*pl) // apunta a la direccion del puntero NULL al final de la lista, sale cuando *pl = NULL
        pl = &(*pl)->sig;

    *pl = nuevo;

    return TODO_OK;
}

void listaVaciarREVISAR(tLista *pl)
{
    tNodo *elim;

    while (*pl)
    {
        elim = *pl;
        *pl = elim->sig; // Avanzás la lista (rompés el enlace antes de liberar)
        free(elim->info);
        free(elim);
    }
}

// funcion para poner en principal_archivo
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