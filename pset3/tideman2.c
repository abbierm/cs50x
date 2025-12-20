#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9
#define string char*

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
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// MY functions
pair make_pair(int a, int b);
void _sort_pairs(int low, int high);
void _merge_pairs(int l, int m, int h);
bool _check_cycles(int win, int los);

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

    // Loop through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Determine if name is the same as inside of the candidates array
        if (strcmp(candidates[i], name) == 0)
        {
            // If so update the ranks with the candidate index
            ranks[rank] = i;
            return true;
        }
    }
    // If none of the candidates are matches, return false
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // iterate through the voter's preferences
    for (int i = 0; i < candidate_count; i++)
    {
        // storing the current candidate's index in candidate's array
        int cur = ranks[i];

        // iterate through the subsequent candidates
        for (int j = i + 1; j < candidate_count; j++)
        {
            // storing the candidate getting compared against
            int comp = ranks[j];

            // add +1 to the candidate at preferences[cur][comp]
            // to show that someone preferred cur over comp
            preferences[cur][comp]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    // a. Iterate through the y-axis of the preferences array
    for (int i = 0; i < candidate_count; i++)
    {
        // Compare ^ to all the next candidates by
        // iterating through the subsequent candidates
        // in the X - axis 
        for (int j = i + 1; j < candidate_count; j++)
        {
            pair new = make_pair(i, j);
            if (new.winner == -1)
                continue;
            else
            {
                pairs[pair_count] = new;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{ 
    int low = 0, high = pair_count;
    _sort_pairs(low, high - 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // iterate through pairs
    for (int i = 0; i < pair_count; i++) {

        // check if pair would result in a cycle
        if (!_check_cycles(pairs[i].winner, pairs[i].loser)){
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

bool _check_cycles(int win, int los)
{
    if (los == win)
        return true;

    // iterate over the rest of the candidates
    for (int i = 0; i < candidate_count; i++) {
        if (locked[i][win]) {
            if (_check_cycles(i, los))
                return true;
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // iterate through locked
    for (int i = 0; i < candidate_count; i++) 
    {
        for (int j = 0; j < candidate_count; j++) 
        {
            if (locked[i][j]) 
            {
                bool win = true;

                // iterate through locked[x][i]
                for (int k = 0; k < candidate_count; k++) {
                    if (k != i && locked[k][i]) {
                        win = false;
                        break;
                    }
                }

                // if any are true set win to false
                if (win) {
                    printf("%s\n", candidates[i]);
                    return;
                }
            }
        }
    }
}

pair make_pair(int a, int b)
{
    int a_v = preferences[a][b], b_v = preferences[b][a];
    pair new_pair;

    if (a_v > b_v)
        new_pair.winner = a, new_pair.loser = b;
    else if (a_v < b_v)
        new_pair.winner = b, new_pair.loser = a;
    else
        new_pair.winner = -1, new_pair.loser = -1;

    return new_pair;
}

void _sort_pairs(int low, int high)
{
    int mid;

    if (low < high)
    {
        int mid = low + (high - low)/ 2;
        _sort_pairs(low, mid);
        _sort_pairs(mid + 1, high);
        _merge_pairs(low, mid, high);
    }
}

void _merge_pairs(int l, int m, int h)
{
    // create two temp arrays
    int leftSize = m - l + 1;
    int rightSize = h - m;
    pair templ[leftSize];
    pair tempr[rightSize];

    // fill up templ with current pairs in-between low and mid
    for (int i = 0; i < leftSize; i++)
        templ[i] = pairs[l + i];

    // fill up tempr with current pairs in-between mid and high
    for (int j = 0; j < rightSize; j++)
        tempr[j] = pairs[m + 1 + j];

    int i = 0, j = 0, k = l;

   while (i < leftSize && j < rightSize) {
        
        int lw = templ[i].winner, ll = templ[j].loser;
        int rw = tempr[j].winner, rl = tempr[j].loser;
        int l_strength = preferences[lw][ll] - preferences[ll][lw];
        int r_strength = preferences[rw][rl] - preferences[rl][rw];

        if (l_strength > r_strength) {
            pairs[k] = templ[i];
            i++;
        }
        else {
            pairs[k] = tempr[j];
            j++;
        }
        k++;
   }

    while (i < leftSize)
    {
        pairs[k] = templ[i];
        i++;
        k++;
    }
    
    while (j < rightSize)
    {
        pairs[k] = tempr[j];
        j++;
        k++;
    }
}
