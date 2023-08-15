// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void insert(int index, node *n);

// TODO: Choose number of buckets in hash table
const unsigned int N = 100;

// Hash table
node *table[N];

// # of words in hash table
int words_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    for (node *cursor = table[hash(word)]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash;

    hash = tolower(word[0]) - 'a' + tolower(word[strlen(word) - 1]) - 'a';

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *words = fopen(dictionary, "r");

    // Unable to open file
    if (words == NULL)
    {
        fclose(words);
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(words, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            // Unable to allocate memory
            fclose(words);
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        int index = hash(word);
        insert(index, n);
        words_count++;
    }
    fclose(words);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
    return true;
}

// Insert node into hash table, takes index and node pointer
void insert(int index, node *n)
{
    n->next = table[index];
    table[index] = n;
}
