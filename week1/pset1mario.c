#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int input;
    do
    {
        input = get_int("Size: ");
    }
    while (input < 1 || input > 8);

    for (int i = 0; i < input; i++)
    {
        for (int k = input - 1; k > i; k--)
        {
            printf(" ");
        }

        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}