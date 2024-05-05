// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 20384;

// Hash table
node *table[N];

// Keeps track of the number of words
int words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int word_hash = hash(word);
    if (table[word_hash] == NULL)
    {
        return false;
    }
    for (node *ptr = table[word_hash]; ptr != NULL; ptr = ptr->next)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
            break;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int v = toupper(word[0]) - '@';
    for (int i = 1; i < 3; i++)
    {
        if (word[i] == '\'')
        {
            v = v * 28;
        }
        else if (word[i] == '\0')
        {
            v = v * 27;
        }
        else
        {
            unsigned int l = toupper(word[i]) - '@';
            v = v * l;
        }
    }
    if (v > N)
    {
        printf("Word: %s hash is %i\n", word, v);
    }

    return v;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // initialize the hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        fclose(dict);
        return false;
    }

    // The string buffer that accepts strings
    char buffer[LENGTH + 1];

    while (fscanf(dict, "%s", buffer) != EOF)
    {
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            fclose(dict);
            return false;
        }

        strcpy(new->word, buffer);

        new->next = NULL;

        unsigned int new_hash = hash(buffer);

        // Adding the new word to the TOP of the list!!!!
        if (table[new_hash] == NULL)
        {
            table[new_hash] = new;
        }
        else
        {

            new->next = table[new_hash];
            table[new_hash] = new;
        }
        words++;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 1; i <= N; i++)
    {
        node *ptr = table[i];
        if (ptr != NULL)
        {
            while (ptr != NULL)
            {
                node *tmp = ptr->next;
                free(ptr);
                ptr = tmp;
            }
        }
        else
        {
            free(ptr);
        }
    }
    return true;
}
