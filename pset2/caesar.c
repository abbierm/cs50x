#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int rotate(char c, int k);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Error: key\n");
            return 2;
        }
    }
    key = atoi(argv[1]) % 26;

    string text = get_string("Plain text:  ");
    printf("ciphertext: ");

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (isalpha(c))
        {
            int upper_flag = 0;
            if (isupper(c))
            {
                upper_flag = 1;
                c = tolower(c);
            }
            c = rotate(c, key);
            if (upper_flag == 1)
            {
                c = toupper(c);
            }
        }
        printf("%c", c);
    }
    printf("\n");
    return 0;
}

char rotate(char c, int k)
{
    if ((c + k) <= 122)
    {
        return c + k;
    }
    else
    {
       int newc = ((k + c) - 123) + 97;
       return newc;
    }
}
