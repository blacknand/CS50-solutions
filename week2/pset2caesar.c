#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check if there are only two command line arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check that each character within the second command line argument is a digit
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Converts argv[1] from string to an int
    int key = atoi(argv[1]);

    // Prompt user for plaintext and print ciphertext
    string plaintext = get_string("Enter plaintext:  ");
    printf("ciphertext: ");

    for (int j = 0; j < strlen(plaintext); j++)
    {
        if (isupper(plaintext[j]))
        {
            // Reset the ASCII value of the element of plain text so it is corresponding to the current index of plaintext
            // Add the key on so it is now shifting the key amount of times
            // Then use mod 26 so it loops back around
            // Add back 65 to bring it back into the alphabet range of ASCII values
            printf("%c", (plaintext[j] - 65 + key) % 26 + 65);
        }
        else if (islower(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 97 + key) % 26 + 97);
        }
        else
        {
            printf("%c", plaintext[j]);
        }
    }
    printf("\n");
}


