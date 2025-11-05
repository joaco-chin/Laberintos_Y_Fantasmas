#include "interno_laberinto.h"
#include "..\comun\codigosRet.h"
#include "interno_fantasma.h"

void generarLaberintoAleatorio(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios,
unsigned *vidasExtra, tLista* listaFantasmas, tPosicion entradaYsalida[])
{
    tPosicion posEnt, posSal;
    int maxBloquesPorPared = 2;

    inicializarMatrizCaracter(matriz, filas, columnas, CAMINO);
    evitarObstruccionesEsquinas(matriz, filas, columnas, maxBloquesPorPared);
    generarEntradaYSalida(matriz, filas, columnas, &posEnt, &posSal);
    evitarObstruccionesEntradaSalida(matriz, filas, columnas, posEnt, posSal, maxBloquesPorPared);
    generarParedesInternas(matriz, filas, columnas, maxBloquesPorPared);
    generarParedesLimite(matriz, filas, columnas, posEnt, posSal);
    colocarCaracteresEspeciales(matriz, filas, columnas, fantasmas, premios, vidasExtra, listaFantasmas);
    actualizarCaracterAlrededorDePosicion(matriz, filas, columnas, posEnt, PARED_RANGO_ENTRADA, CAMINO, maxBloquesPorPared + 1);

    entradaYsalida[0] = posEnt;
    entradaYsalida[1] = posSal;
}

void generarParedesLimite(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal)
{
    if (filas <= 0 || columnas <= 0)
        return;

    // Fila superior (0)
    for (int j = 0; j < columnas; j++)
    {
        if (!((0 == posEnt.fila && j == posEnt.columna) || (0 == posSal.fila && j == posSal.columna)))
            matriz[0][j] = PARED;
    }

    // Fila inferior (filas-1)
    if (filas > 1)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (!(((filas - 1) == posEnt.fila && j == posEnt.columna) || ((filas - 1) == posSal.fila && j == posSal.columna)))
                matriz[filas - 1][j] = PARED;
        }
    }

    // Columna izquierda (0) y columna derecha (columnas-1)
    for (int i = 0; i < filas; i++)
    {
        if (!((i == posEnt.fila && 0 == posEnt.columna) || (i == posSal.fila && 0 == posSal.columna)))
            matriz[i][0] = PARED;

        if (columnas > 1)
        {
            if (!((i == posEnt.fila && (columnas - 1) == posEnt.columna) || (i == posSal.fila && (columnas - 1) == posSal.columna)))
                matriz[i][columnas - 1] = PARED;
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

        if (matriz[posActual.fila][posActual.columna] == CAMINO && !hayCaracterAlrededorDePosicion(matriz, filas, columnas, posActual.fila, posActual.columna, PARED))
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
    listaVaciar(&listaPosLibres);
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

void evitarObstruccionesEsquinas(char **matriz, int filas, int columnas, int maxBloquesPorPared)
{
    for (int j = 0; j <= maxBloquesPorPared; j++)
    {
        matriz[0][j] = PARED;
        matriz[0][columnas - 1 - j] = PARED;
        matriz[filas - 1][j] = PARED;
        matriz[filas - 1][columnas - 1 - j] = PARED;
    }
}

void evitarObstruccionesEntradaSalida(char **matriz, int filas, int columnas, tPosicion posEnt, tPosicion posSal, int maxBloquesPorPared)
{
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
            if (matriz[i][j] == CAMINO)
                matriz[i][j] = caracter;
}

void colocarCaracteresEspeciales(char **matriz, int filas, int columnas, unsigned *fantasmas, unsigned *premios, unsigned *vidasExtra, tLista* listaFantasmas)
{
    unsigned cantFantasmas = colocarCaracteresEnPosicionesAleatorias(matriz, filas, columnas, FANTASMA, *fantasmas, listaFantasmas);
    if (cantFantasmas != *fantasmas)
    {
        printf("Se generaron %d fantasmas en lugar de %d\n", cantFantasmas, *fantasmas);
        *fantasmas = cantFantasmas;
    }
    unsigned cantVidasExtra = colocarCaracteresEnPosicionesAleatorias(matriz, filas, columnas, VIDA_EXTRA, *vidasExtra, listaFantasmas);
    if (cantVidasExtra != *vidasExtra)
    {
        printf("Se generaron %d vidas extra en lugar de %d\n", cantVidasExtra, *vidasExtra);
        *vidasExtra = cantVidasExtra;
    }
    unsigned cantPremios = colocarCaracteresEnPosicionesAleatorias(matriz, filas, columnas, PREMIO, *premios, listaFantasmas);
    if (cantPremios != *premios)
    {
        printf("Se generaron %d premios en lugar de %d\n", cantPremios, *premios);
        *premios = cantPremios;
    }
}

int colocarCaracteresEnPosicionesAleatorias(char **matriz, int filas, int columnas, char caracter, int cantidadCar, tLista* listaFantasmas)
{
    int k = 0;
//    tFantasma fantasma;
    tEntidad fantasma;
    tLista listaPosLibres;
    tPosicion p;
    listaCrear(&listaPosLibres);
    int cantLibres = llenarListaPosicionesLibres(matriz, filas, columnas, &listaPosLibres);
    if (cantLibres == 0)
    {
        listaVaciar(&listaPosLibres);
        return k;
    }

    for (k = 0; k < cantidadCar && cantLibres > 0; k++)
    {
        p = elegirYEliminarPosicionLista(&listaPosLibres, &cantLibres);
        matriz[p.fila][p.columna] = caracter;
        if (caracter == FANTASMA)
        {
            fantasma.y = p.fila;
            fantasma.x = p.columna;
            fantasma.caracterEnt = FANTASMA;
            fantasma.caracterFondo = CAMINO;
            fantasma.estaVivo = FANTASMA_VIVO;
            fantasma.posInicial.fila = p.fila;
            fantasma.posInicial.columna = p.columna;
//            colaEncolar(colaFantasmas, &fantasma, sizeof(tFantasma));
            listaInsertarAlFinal(listaFantasmas, &fantasma, sizeof(tEntidad));
        }
    }
    listaVaciar(&listaPosLibres);
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
                if (listaInsertarAlFinal(lista, &pos, sizeof(tPosicion)) == TODO_OK)
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
                if (listaInsertarAlFinal(lista, &pos, sizeof(tPosicion)) == TODO_OK)
                    count++;
            }
        }
    }
    return count;
}

tPosicion elegirYEliminarPosicionLista(tLista *lista, int *cantidad)
{
    int pos = rand() % (*cantidad); // rango de 0 a cantidad-1
    tPosicion posXY;
    listaRemoverPorPos(lista, &posXY, sizeof(tPosicion), pos);
    (*cantidad)--;
    return posXY;
}
