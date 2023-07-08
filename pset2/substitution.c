#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int check_argument_count(int argument);
int check_argument_key(string key);
char substitution(string key, char user_text);

int main(int argc, string argv[])
{
    // Function that checks command line argument integer number
    int flag_1 = check_argument_count(argc);

    // if flag 1 == False print message and return 1
    if (flag_1 != 0)
    {
        printf("Invalid number of arguments\n");
        return 1;
    }

    // Function that checks the key
    int flag_2 = check_argument_key(argv[1]);
    if (flag_2 != 0)
    {
        printf("Invalid key\n");
        return 1;
    }

    // Ask user for plain_text:
    string plain_text = get_string("Plain text:  \n");

    // Prints ciphertext
    printf("ciphertext: ");

    // For loops that goes through every character of users string
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        // Only submits integers that are valid alphabetical characters
        if (isalpha(plain_text[i]) != 0)
        {
            // Function that performs substitution on each individual character
            char ciphered_char = substitution(argv[1], plain_text[i]);
            printf("%c", ciphered_char);
        }
        // Skips function and returns non-alphabetical character
        else
        {
            printf("%c", plain_text[i]);
        }
    }

    // Prints newline after using substitution key
    printf("\n");
    return 0;
}

int check_argument_count(int argument)
{
    // Returns an error message if it doesn't have 1 argument (message of choice) and returns 1
    if (argument != 2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Function that makes sure the key has 26 valid characters
int check_argument_key(string key)
{
    // Checks length (26 characters)
    int x = strlen(key);
    if (x != 26)
    {
        return 1;
    }

    // Checks to make sure the characters are valid
    for (int i = 0; i < 26; i++)
    {
        // Makes sure key is valid alpha character
        if (isalpha(key[i]) == 0)
        {
            return 1;
        }
        // Turns the current iteration into lowercase to check for duplicates
        char letter = tolower(key[i]);

        // For loop that checks for duplicates by iterating through keys values after the currenet iteration
        for (int j = i + 1; j < 26; j++)
        {
            char comp_letter = tolower(key[j]);
            if (comp_letter == letter)
            {
                return 1;
            }
        }
    }
    return 0;
}

char substitution(string key, char user_text)
{
    // Array of the alphabet
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                       'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
                      };

    // Checks and flags if it is uppercase
    int upper_flag = 0;
    if (isupper(user_text) != 0)
    {
        upper_flag = 1;
        user_text = tolower(user_text);
    }

    // Converts char to integer value
    int integer = user_text;

    // loops through alphabet looking for index number
    for (int i = 0; i < 26; i++)
    {

        // if the integer for the alphabet is the same as the integer for the character
        if (alphabet[i] == integer)
        {
            // We will use the index from the alphabet to get the substitution character
            char sub = key[i];

            // Checks if the character was uppercase
            if (upper_flag == 1)
            {
                // Returns uppercase character
                char upper_sub = toupper(sub);
                return upper_sub;
            }

            // Returns the original sub
            else if (upper_flag == 0)
            {
                char lower_sub = tolower(sub);
                return lower_sub;
            }
        }

    }
    return user_text;
}

