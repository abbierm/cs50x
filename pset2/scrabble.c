#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

struct score_map
{
    int ascii[26];
    int score[26];
    char letter[26];
};

struct score_map make_map(void);
int calculate_score(string word, struct score_map map);

int main(void)
{
    // Prompt Player 1
    string one = get_string("Player 1: ");

    // Prompt player 2
    string two = get_string("Player 2: ");

    // fill map structure
    struct score_map scores = make_map();

    // calculate score for player 1
    int p1_score = calculate_score(one, scores);

    // calculate score for player 2
    int p2_score = calculate_score(two, scores);

    // print winner
    if (p1_score > p2_score)
    {
        printf("Player 1 wins!\n");
    }
    else if (p2_score > p1_score)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }

    return 0;
}

struct score_map make_map(void)
{
    struct score_map new_map;
    int scrabble_scores[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    for (int i = 0; i <= 26; i++)
    {
        new_map.ascii[i] = i + 65;
        new_map.score[i] = scrabble_scores[i];
        new_map.letter[i] = i + 65;
    }

    return new_map;
}

int calculate_score(string word, struct score_map map)
{
    int score = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        char letter = toupper(word[i]);
        for (int j = 0; j <= 26; j++)
        {
            if (letter == map.ascii[j])
            {
                score += map.score[j];
            }
        }
    }
    return score;
}
