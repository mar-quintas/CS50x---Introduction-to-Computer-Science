#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int colemanLiauIndex(string text);

int main(void)
{
    string text = get_string("Text: ");

    int grade = colemanLiauIndex(text);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int colemanLiauIndex(string text)
{
    int letterSum = 0;
    float wordSum = 1;
    int sentenceSum = 0;

    // se repite el ciclo hasta que se recorre toda la longitud del string
    for (int i = 0; i < strlen(text); i++)
    {
        // inicialmente pasamos los caracteres a lowercase
        char character = tolower(text[i]);

        // si el caracter se encuentra entre a y z es una letra y se suma
        if ((character >= 'a') && (character <= 'z'))
        {
            letterSum++;
        }
        // si aparece un espacio asumimos que termino una palabra y comienza otra
        else if (character == ' ')
        {
            wordSum++;
        }
        // Si aparece alguno de estos tres caracteres asumimos que se termino una frase
        else if (character == '?' || character == '!' || character == '.')
        {
            sentenceSum++;

        }

    }

    //Calculo del indice en base a los valores de letterSum, wordSum y sentenceSum
    // index = 0.0588 * L - 0.296 * S - 15.8
    // L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.
    float L = (letterSum * 100) / wordSum;
    float S = (sentenceSum * 100) / wordSum;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    return index;
}