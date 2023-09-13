#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string t);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
// Prompts user for a string of text using get_string
    string copied_text = get_string("Text:");

// Calls on the functions
    float letters = count_letters(copied_text);
    float words = count_words(copied_text);
    float sentences = count_sentences(copied_text);

// Use formula to get the reading index
    float L = (letters / words) * 100;
    float S = (sentences / words) * 100;
    int reading_index = round(.0588 * L - .296 * S - 15.8);

// Print out grade level using the index
    if (reading_index < 16 && reading_index >= 1)
    {
        printf("Grade %i\n", reading_index);
    }
    else if (reading_index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (reading_index < 1)
    {
        printf("Before Grade 1\n");
    }
}

// Counts the number of letters
int count_letters(string t)
{
    int number_letters = 0;
    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (t[i] >= 'A' && t[i] <= 'Z')
        {
            number_letters++;
        }
        else if (t[i] >= 'a' && t[i] <= 'z')
        {
            number_letters++;
        }
    }
    return number_letters;
}

// Counts the numbers of words
int count_words(string text)
{
    // 1 because the last word won't have a space behind it.
    int number_words = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            number_words++;
        }
    }
    return number_words;
}

// Counts the number of sentences
int count_sentences(string text)
{
    int number_sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            number_sentences ++;
        }
    }
    return number_sentences;
}
