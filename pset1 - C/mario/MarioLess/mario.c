#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    int s;

    do
    {
        // User input will determine the size of the pyramid
        n = get_int("Select the size of the pyramid, between 1 and 8\n");
    }
    while (n < 1 || n > 8);

    // Printing i as columns
    for (int i = 1; i <= n; i++)
    {
        s = n - i ;

        // Printing j as characters inside the columns
        for (int j = 0; j < n; j++)
        {
            if (s > j)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}

