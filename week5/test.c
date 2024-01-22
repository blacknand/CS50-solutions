// Test the hash function in dictionary.c
// To compile: clang -o test dictionary.c test.c -lm

#include <stdio.h>
#include "dictionary.h"

int main(void)
{
    char *words[] = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon",
        "mango", "nectarine", "orange", "pear", "quince",
        "raspberry", "strawberry", "tangerine", "watermelon", "blueberry",
        "apricot", "blackberry", "cranberry", "dragonfruit", "guava",
        "kiwifruit", "papaya", "passionfruit", "pomegranate", "raspberry",
        "boysenberry", "cantaloupe", "coconut", "grapefruit", "lime",
        "peach", "plum", "pineapple", "apricot", "blackcurrant",
        "gooseberry", "lychee", "marionberry", "mulberry"
    };

    for (int i = 0; i < 44; i++)
    {
        hash(words[i]);
    }
    
    return 0;
}
