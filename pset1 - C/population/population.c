#include <cs50.h>
#include <stdio.h>

int main(void)
{

    int startSize;
    int endSize;
    int years = 0;

    // Prompts for start size, must be bigger than 9
    do
    {
        startSize = get_int("How many chickens will you have at the beginning?\n");
    }
    while (startSize < 9);

    //Prompts for end size, must be bigger than startSize
    do
    {
        endSize = get_int("How many chickens will you end up having?\n");
    }
    while (endSize < startSize);

    int populationSize = startSize;

    // Calculates the number of years until we reach threshold
    while (endSize > populationSize)
    {
        populationSize = populationSize + populationSize / 3 - populationSize / 4;
        years++;
    };

    // Prints years
    printf("Start Size: %i\n", startSize);
    printf("End Size: %i\n", endSize);
    printf("Years: %i\n", years);

}