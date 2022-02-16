// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// N can be adjusted based on memory. Higher number equal more memory but faster results.
const unsigned int N = 26;

// Int to track amount of words in dictionary
int words = 0;

// Hash table
node *table[N];


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word to obtain a hash value
    int hash_value = hash(word);
    // Access linked list at that index in the hash table
    node *n = table[hash_value];
    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Add ascii values of the words. Means each word is unique based of the letters.
    long sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    // printf("ASCII Value: %li\n", sum);
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open Dictionary File
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Create array for the word
    char word[LENGTH + 1];

    // Read strings from file one at a time
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Not enough memory to allocate node");
            return false;
        }
        // Copy this word into node
        // printf("%s\n", word);
        strcpy(n->word, word);
        n->next = NULL;

        // Hash word to obtain a hash value
        int hash_value = hash(word);

        // Insert node into hash table at that location
        n->next = table[hash_value];
        table[hash_value] = n;

        words++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Grabs the word count that was previously added in load() and returns it.
    // printf("Words in dictionary: %i\n", words);
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Go through the table and free every saved word from the memory.
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];
        
        while (n != NULL)
        {
            node *freeme = n;
            n = n->next;
            free(freeme);
        }
        if (n == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
