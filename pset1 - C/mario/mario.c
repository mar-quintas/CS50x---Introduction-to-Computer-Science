#include <cs50.h>
#include <stdio.h>

void leftPyramidRow(int spaces, int numColumns);
void rightPyramidRow(int hashes, int numColumns);

int main(void)
{
    int numColumns;
    int spaces;

    do
    {
        // User input will determine the size of the pyramid as numColumns
        numColumns = get_int("Select the size of the pyramid, between 1 and 8\n");
    }
    while (numColumns < 1 || numColumns > 8);

    // Printing i as columns
    for (int i = 1; i <= numColumns; i++)
    {
        spaces = numColumns - i ;

        leftPyramidRow(spaces, numColumns);

        printf("  ");

        rightPyramidRow(i, numColumns);

        printf("\n");

    }
}

void rightPyramidRow(int hashes, int numColumns)
{
    // Printing j as rows inside the columns on the right side of the pyramid

    for (int j = 0;  j < hashes && j < numColumns; j++)
    {
        printf("#");
    }
}

void leftPyramidRow(int spaces, int numColumns)
{
    // Printing j as rows inside the columns on the left side of the pyramid
    for (int j = 0; j < numColumns; j++)
    {
        if (spaces > j)
        {
            printf(" ");
        }
        else
        {
            printf("#");
        }
    }
}