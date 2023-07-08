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
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool cycle_pairs(int winner, int loser);
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
        // Creates the array based on how many candidates there are
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
    // For loop for each canidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Checks to see if candidate is valid
        if (strcmp(name, candidates[i]) == 0)
        {
            // variable for the canidates index
            int index = i;

            // Updates the ranks array
            ranks[rank] = i;

            // returns true
            return true;

            // breaks the for loop
            break;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = candidate_count; i >= 0; i--)
    {
        // Variables to help me keep track of the candidates
        int candidate_index = i;
        int candidate = ranks[i];

        // For loop to compare the other candidates to the current one in rank array
        for (int j = 0; j < candidate_count; j++)
        {
            // More variables to help me keep track of the candidates
            int compared_index = j;
            int compared_candidate = ranks[j];

            // Conditional statement to compare the candidates
            if (candidate_index < compared_index)
            {
                // Adds a vote to that candidates
                preferences[candidate][compared_candidate]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // iterates through the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // iterates through who we are compairing them to
        for (int j = 0; j < candidate_count; j++)
        {
            // conditional statement that checks for majority preference
            if (preferences[i][j] > preferences[j][i])
            {
                // updates pairs struct with winner/losers
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // iterates through the pairs
    for (int i = 0; i < pair_count - 1; i++)
    {
        pair cache;

        // iterates through a second time for comparision
        {
            for (int j = 0; j < pair_count; j++)
            {
                // Conditional to check if pair strength of j is < than j + 1
                if (preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner] < preferences[pairs[j +
                        1].winner][pairs[j + 1 ].loser] - preferences[pairs[j + 1].loser][pairs[j + 1].winner])
                {
                    // Switches j and j + 1
                    cache = pairs[j];
                    pairs[j] = pairs[j + 1];
                    pairs[j + 1] = cache;
                }
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Iterates through the sorted_pairs
    for (int i = 0; i < pair_count; i++)
    {
        // Variables to help me keep track of things
        int winner = pairs[i].winner; // Index #s of the candidates
        int loser = pairs[i].loser;

        // Cycles through the pairs of winners and losers
        if (!(cycle_pairs(winner, loser)))
        {
            locked[winner][loser] = true;
        }

    }

    return;
}

// Uses index numbers to cycle through the preferences to see if there is a case where the loser gets circled back to the winner
bool cycle_pairs(int winner, int loser)
{
    // If  int winner = int loser they are the same
    if (winner == loser)
    {
        // Says that it IS a cycle so returns true
        return true;
    }

    // iterates through seeing if there is a case where the loser could be the winner
    for (int i = 0; i < candidate_count; i++)
    {
        // Checks to see if ALREADY has been locked TRUE
        if (locked[loser][i] == true)
        {
            // Goes back and recursively checks to see if the "loser's loser" ever beats someone who beats the winner
            if (cycle_pairs(winner, i))
            {
                return true;
            }
        }
    }
    // If no cycles returns false
    return false;

}

// Print the winner of the election
void print_winner(void)
{
    // Iterates through the candidates x axis
    for (int i = 0; i < candidate_count; i++)
    {
        // bool flag to use to check the candidates
        bool flag = false;

        // Iterates through the candidates y axis
        for (int j = 0; j < candidate_count; j++)
        {
            // Checks if locked_pairs[j][i] is true
            if (locked[j][i] == true)
            {
                // If locked[j][i] is true, it means candidates[i] can't be the winner bc is a winner over them and we already checked to make sure there were no cycles
                flag = true;
                break;
            }
        }
        //
        if (flag == true)
        {
            // Continues through the loop
            continue;
        }
        // Winner because noone was locked over them
        else
        {
            printf("%s\n", candidates[i]);
        }
    }
}
