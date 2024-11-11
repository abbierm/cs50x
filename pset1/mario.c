#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Outer Loop controls which row is getting printed
    for (int i = 0; i < height; i++)
    {
         // 1. Prints the alignment spaces
        for (int spaces = height - i; spaces > i; spaces--)
        {
            printf(" ");
        }
        // 2. Prints the hashes
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
         // 3. Prints two spaces for the middle of the pyramid
        printf("  ");

        // 4. Prints the right hashes
        for (int q = 0; q <= i; q++)
        {
            printf("#");
        }
        // 5. Prints a new line for the next row. 
        printf("\n");
    }
}
