// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
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

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * LENGTH;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int word_hash = hash(word);
    bool found = false;

    for (node* ptr = table[word_hash]; ptr != NULL && !found; ptr = ptr->next)
    {
        found = strcasecmp(ptr->word, word) == 0;
        if (found) break;
    }

    return found;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word) - 1; i < n; i++)
    {
        if (!isalpha(word[i])) continue;
        hash += tolower(word[i]);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL) return false;

    // Get file size;
    fseek(dict, 0, SEEK_END);
    long file_size = ftell(dict);
    fseek(dict, 0, SEEK_SET);

    while (ftell(dict) < file_size)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            free(new_node);
            return false;
        }

        fgets(new_node->word, LENGTH + 1, dict);

        if (new_node->word[strlen(new_node->word) - 1] == '\n')
        {
            new_node->word[strlen(new_node->word) - 1] = '\0';
        }

        if (strlen(new_node->word) == 0)
        {
            free(new_node);
            continue;
        }

        // Gets the hash of the word;
        unsigned int word_hash = hash(new_node->word);

        new_node->next = table[word_hash];
        table[word_hash] = new_node;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int sum = 0;

    for (int i = 0; i < N; i++)
    {
        for (node *ptr = table[i]; ptr != NULL; ptr = ptr->next)
        {
            sum++;
        }
    }

    return sum;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];

        while (ptr != NULL)
        {
            node *next = ptr->next;
            free(ptr);
            ptr = next;
        }

        free(ptr);
    }
    return true;
}
