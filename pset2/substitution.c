#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct keyMap
{
    char c[52];
    char s[52];
};

bool check_argument_count(int args);
bool check_key_length(string key);
bool check_key(string key);
struct keyMap make_map(string key);
void cipher_text(struct keyMap keymap, string text);

int main(int argc, string argv[])
{
    if (!check_argument_count(argc))
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    if (!check_key(argv[1]) || !check_key_length(argv[1]))
    {
        printf("Key must contain all 26 characters\n");
        return 1;
    }

    string text = get_string("Text: ");

    string k = argv[1];
    struct keyMap map = make_map(k);
    printf("ciphertext: ");
    cipher_text(map, text);

    return 0;
}

bool check_argument_count(int args)
{
    if (args != 2)
    {
        return false;
    }
    return true;
}

bool check_key_length(string key)
{
    int n = strlen(key);
    if (n == 26)
    {
        return true;
    }
    return false;
}

bool check_key(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        char c = key[i];
        if (isalpha(c) == 0)
        {
            return false;
        }
        for (int j = i + 1; j <= n; j++)
        {
            char comp = key[j];
            if (c == comp)
            {
                return false;
            }
        }
    }
    return true;
}

struct keyMap make_map(string key)
{
    struct keyMap new_map;
    for (int i = 0; i < 26; i++)
    {
        new_map.c[i] = i + 65;
        new_map.c[i + 26] = 97 + i;
        new_map.s[i] = toupper(key[i]);
        new_map.s[i + 26] = tolower(key[i]);
    }

    return new_map;
}

void cipher_text(struct keyMap keymap, string text)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (c >= 65 && c <= 90 | c >= 97 && c <= 122)
        {
            for (int j = 0; j <= 52; j++)
            {
                if (c == keymap.c[j])
                {
                    printf("%c", keymap.s[j]);
                    break;
                }
            }
        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
    return;
}

