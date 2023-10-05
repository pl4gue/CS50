#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int cents;

    do
    {
        printf("Troca devida: ");
        scanf("%i", &cents);
    }
    while (cents <= 0);

    int moedas[] = {25, 10, 5, 1};
    int resultado = 0;

    for (int i = 0; i < 4; i++)
    {
        resultado += cents / moedas[i];
        cents %= moedas[i];
    }

    printf("%d\n", resultado);
}
