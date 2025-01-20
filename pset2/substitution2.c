// Substitution version with my own get_string() function.

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef char* string;

string upper_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string lower_alphabet = "abcdefghijklmnopqrstuvwxyz";

string get_string(string s, int size);
bool check_key_length(string k);
bool check_key_letters(string k);
void cipher_text(string k, string t);

int main(int argc, string argv[])
{
    
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    string key = argv[1];
    if (!check_key_length(key) || !check_key_letters(key))
    {
        return 2;
    }

    char t[100];
    string text = get_string(t, sizeof(t));
    cipher_text(key, text);

    return 0;
}

string get_string(string s, int size)
{
    printf("Enter text: ");
    string t = fgets(s, size, stdin);
    if (t == NULL)
    {
        printf("fgets error\n");
        return NULL;
    }
    char *np = strchr(t, '\n');
    if (np == NULL)
    {
        printf("Input needs to be shorter than 100 lines\n");
        return NULL;
    }
    *np = '\0';
    return t;
}

bool check_key_length(string k)
{
    int length = strlen(k);
    if (length != 26)
    {
        printf("key should only be 26 letters long\n");
        return false;
    }
    return true;
}

bool check_key_letters(string k)
{
    for (int i = 0; i < 27; i++)
    {
        char cur = k[i];
        for (int j = i + 1; j < 27; j++)
        {
            if (cur == k[j])
            {
                printf("Key should not include duplicate letters\n");
                return false;
            }
        }
    }
    return true;
}

void cipher_text(string k, string t)
{
    printf("Ciphertext: ");
    for (int i = 0, n = strlen(t); i < n; i++)
    {
        char c = t[i];
        if (!isalpha(c))
        {
            printf("%c", c);
            continue;
        }
        for (int j = 0, m = strlen(upper_alphabet); j < m; j++)
        {
            if (c == upper_alphabet[j])
            {
                printf("%c", toupper(k[j]));
            }
            else if (c == lower_alphabet[j])
            {
                printf("%c", tolower(k[j]));
            }
        } 
    }
    printf("\n");
}
