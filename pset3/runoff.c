#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // checks name
    for (int x = 0; x < candidate_count; x++)
    {
        if (strcmp(name, candidates[x].name) == 0)
        {
            // Updates preferences based on rank
            int candidate_index = x;
            preferences[voter][rank] = candidate_index;
            return true;
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Loops through number of voters
    for (int i = 0; i < voter_count; i++)
    {
        // Loop through the voters preferences
        for (int j = 0; j < candidate_count; j++)
        {
            // links their jth preference with the correct index number
            int voter_preference = preferences[i][j];

            // Checks to make sure the candidate hasn't been eliminated
            if (candidates[voter_preference].eliminated == false)
            {
                // Updates the candidates vote count
                candidates[voter_preference].votes++;
                break;
            }
        }
    }
    return;
}



// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Variable to store max votes
    int most_votes = 0;
    int winner_index;

    // Loops through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Checks which candidate has the most votes
        if (candidates[i].votes > most_votes)
        {
            // Updates the "winning candidate"
            most_votes = candidates[i].votes;
            winner_index = i;
        }
    }
    // Checks to make sure winner has the majority
    float fifty_percent_of_votes = voter_count / 2;
    if (most_votes > fifty_percent_of_votes)
    {
        printf("%s\n", candidates[winner_index].name);
        return true;
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Variable to store smallest number of votes
    int min_votes;

    // Loops through every candidates struct
    for (int i = 0; i < candidate_count; i++)
    {
        // checks if minimum is the smallest number
        if (candidates[i].votes < min_votes && candidates[i].eliminated == false)
        {
            // Updates the smallest number to new smallest number
            min_votes = candidates[i].votes;
        }
    }

    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Variable that counts the number tied
    int number_tied = 0;

    // Variable that stores number of viable candidates
    int valid_candidates = 0;

    // Loops through the canidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Checks if the votes is equal to the minimum number
        if (candidates[i].votes == min)
        {
            // Adds to variable to see the number of viable candidates
            if (candidates[i].eliminated == false)
            {
                number_tied++;
                valid_candidates++;
            }
        }
        // Adds to viable candidates if number is greated than the min
        else if (candidates[i].votes > min)
        {
            valid_candidates++;
        }
    }
    if (valid_candidates == number_tied)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Loops through the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Makes sure candidate has not already been eliminated
        if (candidates[i].eliminated == false)
        {
            // Checks if candidates has the lowest number
            if (candidates[i].votes == min)
            {
                // Turns eliminated to true
                candidates[i].eliminated = true;
            }
        }
    }
    return;
}
