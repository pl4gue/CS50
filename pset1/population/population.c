#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startSize, endSize, years; // TODO: Prompt for start size
    years = 0;

    do {
     startSize = get_int("Start size?");
    } while (startSize < 9);

    // TODO: Prompt for end size
    do {
     endSize = get_int("End size?");
    } while (endSize < startSize);

    // TODO: Calculate number of years until we reach threshold
    while (startSize < endSize) {
        startSize += (startSize / 3) - (startSize / 4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}
