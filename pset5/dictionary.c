// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 356;

// Hash table
node *table[N];

// Number of words
unsigned int words;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Use hash function to get index
    unsigned int index = hash(word);

    // Iterate through the linked list
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp ->next)
    {
        // Use strcasecmp to check every word while next != NULL
        if (strcasecmp(word, tmp->word) == 0)
        {
            // If both words are the same return true
            return true;
            break;
        }
    }

    // Else return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Improved Hash Function
    unsigned int hash_index = 0;
    int i = 0;
    while (word[i] != '\0' && i < 3)
    {
        if (word[i] != 39)
        {
            hash_index += (toupper(word[i]) - '@');
        }
        i++;
    }

    return hash_index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open Dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    // Buffer that stores the word
    char w[LENGTH + 1];

    // Read strings from file 1 word at a time
    while (fscanf(dict, "%s", w) != EOF)
    {
        // Allocates memory for next word in dict
        node *new_entry = malloc(sizeof(node));

        // Check if the return value is NULL
        if (new_entry == NULL)
        {
            return false;
        }

        // Copy word into node using strcpy
        strcpy(new_entry->word, w);
        new_entry->next = NULL;

        // Hash word to create a hash value
        unsigned int index = hash(w);

        // Points Hash bucket to the new item on the list
        new_entry->next = table[index];
        table[index] = new_entry;

        // counter for size
        words++;

    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Uses counter in the load function
    return words;

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through the hash table
    for (unsigned int i = 0; i < N; i++)
    {
        node *marker = table[i];

        // Iterate through the linked lists
        while (marker != NULL)
        {
            node *tmp = marker;
            marker = marker->next;
            free(tmp);
        }

        if (marker == NULL && i == N - 1)
        {
            return true;
        }
    }

    return false;
}

