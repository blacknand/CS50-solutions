#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!!\n");
    }
    else
    {
        printf("It is a tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int score = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        // A is 65 so if the first index of work was A then it means 65 - 65
        // which will then be used to access the corresponding element of the POINTS
        // array which is POINTS[0]

        // ASCII value - 65 is where the element is in the POINTS array

        // Index of the word is according to ASCII
        // Which position of the POINTS array should it be?
        // What ever the index is in ASCII which is word[i] - 65
        // Index in ASCII - 65 will give us the index in the POINTS array

        // If the word is uppercase
        if (isupper(word[i]))
        {
            score = score + POINTS[word[i] - 65];
        }
        // If the word is lowercase
        if (islower(word[i]))
        {
            score = score + POINTS[word[i] - 97];
        }

    }
    return score;
}

