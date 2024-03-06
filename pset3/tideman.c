#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
int find_strength(pair p);
void sort_pairs(void);
bool check_cycles(int winner, int loser);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // current rank is i
        // current person is ranks[i]
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int p = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] != preferences[j][i])
            {
                pair new_pair;
                p += 1;
                if (preferences[i][j] > preferences[j][i])
                {
                    new_pair.winner = i;
                    new_pair.loser = j;
                }
                else
                {
                    new_pair.winner = j;
                    new_pair.loser = i;
                }
                pairs[p - 1] = new_pair;
            }
        }
    }
    pair_count = p;
    return;
}

// Finds the difference between winning votes and losing votes for preferences
int find_strength(pair p)
{
    int winner_votes = preferences[p.winner][p.loser];
    int loser_votes = preferences[p.loser][p.winner];
    int strength = winner_votes - loser_votes;
    return strength;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        int current_strength = find_strength(pairs[i]);

        // Index for the current highest strength pair
        int current_top_index = i;

        // Comparing Loop
        for (int j = i + 1; j < pair_count; j++)
        {
            int compare_strength = find_strength(pairs[j]);
            if (compare_strength > current_strength)
            {
                current_top_index = j;
                current_strength = compare_strength;
            }
        }
        pair temp = pairs[i];
        pairs[i] = pairs[current_top_index];
        pairs[current_top_index] = temp;
    }

    return;
}

// Literally checking them all to see if at any point the start will be the
// the same as the loser.
bool check_cycles(int winner, int loser)
{
    // printf("Checking cycles for %i-%i\n", winner, loser);
    // printf("Checking cycles for %i %i\n", start, loser);
    if (winner == loser)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner] == true)
        {
            // printf("locked[%i][%i] is true.\n", i, winner);
            if (check_cycles(i, loser))
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        // printf("Checking pair: %i - %i\n", pairs[i].winner, pairs[i].loser);
        if (!(check_cycles(pairs[i].winner, pairs[i].loser)))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            // printf("locking Pairs %i - %i\n", pairs[i].winner, pairs[i].loser);
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int flag = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                flag = 1;
            }
        }
        if (flag == 0)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}
