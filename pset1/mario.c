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
    while (height <= 0 || height > 8);

    for (int i = 1; i <= height; i++)
    {
        for (int as = height - i; as > 0; as--)
        {
            printf(" ");
        }
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("  ");

        for (int x = 0; x < i; x++)
        {
            printf("#");
        }
        printf("\n");
    }
}
