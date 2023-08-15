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
void print_winner(void);
bool will_cycle(int winner, int loser);

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
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            ++preferences[ranks[i]][ranks[j]];
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int pref1 = preferences[i][j]; // Prefer i over j
            int pref2 = preferences[j][i]; // Prefer j over i

            if (pref1 == pref2) // No preference for i or j
            {
                continue;
            }

            pair this_pair;

            if (pref1 > pref2) // More prefer i over j
            {
                this_pair.winner = i;
                this_pair.loser = j;
            }
            else // More prefer j over i
            {
                this_pair.winner = j;
                this_pair.loser = i;
            }
            pairs[pair_count] = this_pair;
            ++pair_count;
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble sort
    for (int i = 0, swaps = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - i; j++)
        {
            pair pair1 = pairs[j];
            pair pair2 = pairs[j + 1];
            int win1 = preferences[pair1.winner][pair1.loser] - preferences[pair1.loser][pair1.winner];
            int win2 = preferences[pair2.winner][pair2.loser] - preferences[pair2.loser][pair2.winner];

            if (win1 < win2) // if pair j has weaker vicotry strength than the next pair
            {
                pairs[j + 1] = pair1;
                pairs[j] = pair2;
                ++swaps;
            }
        }
        // If pairs array is sorted after going through (no swaps)
        if (swaps == 0)
        {
            break;
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        pair pair = pairs[i];
        if (!will_cycle(pair.winner, pair.loser)) // If no cycle
        {
            locked[pair.winner][pair.loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                continue;
            }
            // If edge j to i exists, then i is not source
            if (locked[j][i])
            {
                is_source = false;
                break;
            }
        }
        if (is_source)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
}

// Given a winner and loser, return if adding this edge will create a cycle in locked
bool will_cycle(int winner, int loser)
{
    // Recursive
    if (winner == loser)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            return will_cycle(i, loser);
        }
    }
    return false;
}