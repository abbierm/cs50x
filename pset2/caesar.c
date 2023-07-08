#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int rotate(char c, int rotate_number);

int main(int argc, string argv[])
{
    // if int argc != 2, print an error message and and return 1
    if (argc != 2)
    {
        printf("Program needs an integer value and you gave %i\n", argc - 1);
        return 1;
    }

    // Input validation to make sure the argument is a digit;
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isdigit(argv[1][i]))
        {
            continue;
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    int key = atoi(argv[1]) % 26;

    // Gets plain text string
    string plain_text = get_string("Plain text:  ");
    printf("ciphertext: ");

    // Use rotate function
    // n needs to be less than strlen so we don't include the null char
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        char letter = plain_text[i];
        int ciphertext = rotate(letter, key);

        // Output
        printf("%c", ciphertext);
    }

    printf("\n");
}

//function that moves the char by the digits the user provided
int rotate(char c, int rotate_number)
{
    if (c >= 65 && c <= 90)
    {
        int new_upper_char = c + rotate_number;
        if (new_upper_char > 90)
        {
            int difference = new_upper_char - 90;
            new_upper_char = 64 + difference;
            return new_upper_char;
        }
        else
        {
            return new_upper_char;
        }
    }
    else if (c > 96 && c <= 122)
    {
        int new_lower_char = c + rotate_number;
        if (new_lower_char > 122)
        {
            int upper_diff = new_lower_char - 122;
            new_lower_char =  96 + upper_diff;
            return new_lower_char;
        }
        else
        {
            return new_lower_char;
        }
    }
    else
    {
        return c;
    }

}
