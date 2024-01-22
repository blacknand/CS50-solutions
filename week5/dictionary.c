// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
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

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Counter for the number of words in the dictionary
int word_counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    if (word == NULL)
    {
        return false;
    }

    // TODO
    int pos = hash(word);
    for (node *ptr = table[pos]; ptr != NULL; ptr = ptr->next)
    {
        if (strcasecmp(ptr->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    if (word == NULL)
    {
        return 1;
    }

    // TODO: Improve this hash function
    int total = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        total += toupper(word[i]) * pow(5, i);
    }
    total %= N;
    return total;
    // return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");

    // Check if the file is valid
    if (source == NULL)
    {
        return 1;
    }

    // Read each word in the file
    char temp[LENGTH + 1];
    while (fscanf(source, "%s", temp) != EOF)
    {
        word_counter++;
        // Create space for a new hash table node
        node *n = malloc(sizeof(node));

        // Check if there is enough memory
        if (n == NULL)
        {
            return false;
        }

        // Copy the word into the new node
        strcpy(n->word, temp);
        n->next = NULL;

        // Hash the word to obtain its hash value
        int pos = hash(n->word);

        // Insert the new node into the hash table
        n->next = table[pos];
        table[pos] = n;
    }

    // Close the dictionary file
    fclose(source);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_counter;
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
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}

void test_print(void)
{
    for (int i = 0; i < N; i++)
    {
        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
        {
            printf("%s\n", tmp->word);
        }
    }
}