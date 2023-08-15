#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Ask user for height
    int height;
    do
    {
        height = get_int("How tall should the pyramids be? (1 to 8 inclusive): ");
    }
    while (height < 1 || height > 8);

    // Print the pyramids
    for (int i = 0; i < height; i++)
    {
        // Spaces
        for (int j = 0; j < (height - i - 1); j++)
        {
            printf(" ");
        }
        // Bricks
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("  ");

        // Bricks
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}