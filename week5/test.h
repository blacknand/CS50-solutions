// Header file to test the hash function in dictionary.c

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

const unsigned int N = 26;
node *table[N];

unsigned int hash(const char *word);
