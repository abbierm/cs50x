#include <stdbool.h>
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
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// MY functions
int find_pair_strength(pair p);
void _sort_pairs(int l, int r);
void merge_pairs(int l, int m, int r);
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
void add_pairs(void) {
    for (int i = 0; i < candidate_count; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            int op = j, cur = i;
            int cur_prefs = preferences[i][j], op_prefs = preferences[j][i];
            if (cur_prefs != op_prefs) {
                pair new_pair;
                if (cur_prefs > op_prefs)
                    new_pair.winner = cur, new_pair.loser = op;
                else
                    new_pair.winner = op, new_pair.loser = cur;
                pairs[pair_count] = new_pair;
                pair_count++;
            }
        }
    }
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

int find_pair_strength(pair p) {
    int s =  preferences[p.winner][p.loser] - preferences[p.loser][p.winner];
    // printf("Strength for %i - %i: %i\n", p.winner, p.loser, s);
    return s;
}

void merge_sort(int l, int m, int r) {
    // create temp arrays
    int l_size = m - l + 1;
    int r_size = r - m;

    pair ltemp[l_size];
    pair rtemp[r_size];

    for (int i = 0; i < l_size; i++) {
        ltemp[i] = pairs[l + i];
    }
    for (int j = 0; j < r_size; j++) {
        rtemp[j] = pairs[m + j + 1];
    }

    int lc = 0, rc = 0;
    int c = l;

    while (lc < l_size && rc < r_size) {
        pair lpair = ltemp[lc], rpair = rtemp[rc];
        int lstrength = find_pair_strength(lpair);
        int rstrength = find_pair_strength(rpair);
        if (lstrength >= rstrength) {
            pairs[c] = ltemp[lc];
            lc++;
        }
        else {
            pairs[c] = rtemp[rc];
            rc++;
        }
        c++;
    }

    while (lc < l_size) {
        pairs[c] = ltemp[lc];
        c++;
        lc++;
    }

    while (rc < r_size) {
        pairs[c] = rtemp[rc];
        rc++;
        c++;
    }
}

// sorts pairs array slice from s to l
void _sort_pairs(int l, int r) {
    if (r > l) {

        int m = ((r - l) / 2) + l;
        _sort_pairs(l, m);
        _sort_pairs(m + 1, r);
        merge_sort(l, m, r);
    }
}

void sort_pairs(void) {
    _sort_pairs(0, pair_count - 1);
}
