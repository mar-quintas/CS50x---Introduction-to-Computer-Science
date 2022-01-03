#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>


// This program enables you to encrypt messages using a subtitution cipher
// should recieve by comand line an argument provinding the key
int main(int a, string key[])
{
    if (a == 1)
    {
        printf("Usage: ./substitution key.\n");
        return 1;
    }
    else if (strlen(key[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        for (int i = 0; i < (strlen(key[1])); i++)
        {
            key[1][i] = tolower(key[1][i]);

            if (key[1][i] < 'a' || key[1][i] > 'z')
            {
                printf("Invalid character in key\n");
                return 1;
            }
        }

        char checkedLetters[strlen(key[1])];

        checkedLetters[0] = key[1][0];

        for (int i = 1; i < strlen(key[1]); i++)
        {
            for (int j = 0; j < strlen(checkedLetters); j++)
            {
                if (checkedLetters[j] == key[1][j + 1 ])
                {
                    printf("Duplicate character in key\n");
                    return 1;
                }
            }
            checkedLetters[i] = key[1][i];
        }
    }

//    string key[] = "YTNSHKVEFXRBAUQZCLWDMIPGJO";

    //Palabra a cifrar
    string userInput = get_string("plaintext: ");

    //Palabra ya cifrada
    char cipherOutput[strlen(userInput) + 1];

    // Variable temporal para buscar en la variable KEY la letra a usar
    char searchKey;

    // Ciclar por toda la longitud del string
    for (int i = 0; i < (strlen(userInput)); i++)
    {
        // Si es true significa que esta en miniscula
        if (islower(userInput[i]) != 0)
        {
            int index = userInput[i] - 'a';
            searchKey = tolower(key[1][index]);

        }
        //Sino chequear mayusculas
        else if (isupper(userInput[i]) != 0)
        {
            int index = userInput[i] - 'A';
            searchKey = toupper(key[1][index]);
        }
        // Sino es un simbolo y no se modifica
        else
        {
            searchKey = userInput[i];
        }

        cipherOutput[i] = searchKey;

    }
    cipherOutput[strlen(userInput)] = '\0';

    printf("ciphertext: %s\n", cipherOutput);

}