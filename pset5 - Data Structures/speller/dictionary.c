// Implements a dictionary's functionality

#include <stdbool.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 30000;

// Hash table
node *table[N];

// size counter
unsigned int size_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);

    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash function modified from https://stackoverflow.com/questions/14409466/simple-hash-functions

    unsigned int hashAddress = 0;

    for (unsigned int counter = 0; word[counter] != '\0'; counter++)
    {
        hashAddress = hashAddress * tolower(word[counter]) + tolower(word[counter]) + counter;
    }
    return (hashAddress % (N - 1));

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    // Open my dictionary file in read mode
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char tmp[LENGTH + 1];

    while (fscanf(dict, "%s", tmp) != EOF)
    {
        // Allocate memory for one node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, tmp);

        int index = 0;
        index = hash(tmp);

        n->next = table[index];
        table[index] = n;
        size_counter += 1;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return size_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i <= N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }

    }
    return true;
}
