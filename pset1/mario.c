#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Makes sure user puts in valid character
    do
    {
        // Ask user for height
        height = get_int("Pick a height between 1 and 8: ");
    }
    while (height < 1 || height > 8);

    // For loop to print the mario pyramid

    for (int i = 0; i < height; i++)
    {
        // Prints the alignment spaces
        for (int alignmentSpaces = height - 1; alignmentSpaces > i; alignmentSpaces--)
        {
            printf(" ");
        }
        // Prints the hashes
        for (int j = height ; j >= height - i ; j--)
        {
            printf("#");
        }
        // Spaces in between the hashes
        printf("  ");

        // Prints next row of hashes
        for (int q = height ; q >= height - i; q--)
        {
            printf("#");
        }
        printf("\n");

    }

}
