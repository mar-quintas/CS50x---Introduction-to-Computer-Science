#include <cs50.h>
#include <stdio.h>
#include <string.h>



int main(void)
{

#define SIZE 6

    int toSort[SIZE] = {10,15,4,6,9,2};

    for (int i = 0; i < SIZE -1; i++)
    {
        for (int j = 0; j < SIZE - i -1; j++)
        {
            if (toSort[j] > toSort[j+1])
            {
                int temp = toSort[j];
                toSort[j] = toSort[j+1];
                toSort[j+1] = temp;
            }
        }
    }



    printf("La lista ordenada queda: \n");
    for (int i = 0; i < SIZE; i++)
    {
         printf("[%d] %d \n", i, toSort[i]);

    }

}

