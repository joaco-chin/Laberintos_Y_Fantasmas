#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define VELOCIDAD_JUEGO 1000

char ingresarTeclaDeJugador(unsigned periodo)
{
    clock_t temp = clock() + periodo;
    char entrada = 0;

    fflush(stdin);
    while(temp > clock())
    {
        if(kbhit())
        {
            entrada = getch();
            return entrada;
        }
    }

    return entrada;
}

int main()
{
    char tecla;

    tecla = ingresarTeclaDeJugador(VELOCIDAD_JUEGO);
    if(tecla != 0)
        printf("%c", tecla);
    else
        puts("Se ha terminado el tiempo");
    return 0;
}
