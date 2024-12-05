#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

float count_letters(string text);
float count_words(string text);
float count_sentences(string text);
int calculate_index(float words, float sentences, float letters);

int main(void)
{
    string text = get_string("Text: ");
    float letters = count_letters(text);
    float words = count_words(text);
    float sentences = count_sentences(text);
    int index = calculate_index(words, sentences, letters);
    if (index > 0 && index <= 16)
    {
        printf("Grade %i\n", index);
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade 16+\n");
    }
}

float count_letters(string text)
{
    float letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = toupper(text[i]);
        if (c >= 65 && c <= 90)
        {
            letters += 1;
        }
    }
    return letters;
}

float count_words(string text)
{
    float words = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            words += 1;
        }
    }
    return words;
}

float count_sentences(string text)
{
    float sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (c == '.' || c == '?' || c == '!')
        {
            sentences += 1;
        }
    }
    return sentences;
}

int calculate_index(float words, float sentences, float letters)
{
    float avg_letters = (letters / words) * 100;
    float avg_sentences = (sentences / words) * 100;
    float index = (.0588 * avg_letters) - (.296 * avg_sentences) - 15.8;
    int i = round(index);
    return i;
}
