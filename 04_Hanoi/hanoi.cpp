/*
    Data Structures 2023-2024 Lab 04: Hanoi

    @author Krish Arora
*/
#include <vector>

using namespace std;

typedef vector<int> VI;
typedef vector<long> VL;
vector<VL> n_hanoi;
vector<VI> k_hanoi;

int MAX_TOWERS = 10;
int MAX_DISKS = 10000;

// Initialize n_hanoi and k_hanoi
void hanoi_init()
{
    // Initialize first two unused towers
    for (int twr = 0; twr < 2; twr++)
    {
        n_hanoi.push_back(VL(MAX_DISKS + 1));
        k_hanoi.push_back(VI(MAX_DISKS + 1));
    }

    for (int twr = 2; twr <= MAX_TOWERS; twr++)
    {
        // Initialize n_hanoi and k_hanoi for the current number of towers
        n_hanoi.push_back(VL(MAX_DISKS + 1));
        k_hanoi.push_back(VI(MAX_DISKS + 1));

        // Base case: 0 moves for 0 disks
        n_hanoi[twr][0] = 0;
        k_hanoi[twr][0] = 0;

        for (int dsks = 1; dsks <= MAX_DISKS; dsks++)
        {
            // Single disk case
            if (dsks == 1)
            {
                n_hanoi[twr][dsks] = dsks;
                k_hanoi[twr][dsks] = 0;
                continue;
            }

            // Two-tower (classical) case
            if (twr == 2)
            {
                n_hanoi[twr][dsks] = (dsks < 2) ? dsks : -1;
                continue;
            }

            n_hanoi[twr][dsks] = -1;

            for (long k = 1; k < dsks; k++)
            {
                // Check and calculate moves for splitting at k disks
                long prev_move = n_hanoi[twr - 1][dsks - k];
                long current_move = n_hanoi[twr][k];

                if (prev_move == -1 || current_move == -1)
                {
                    continue;
                }
                long move_twice = 2 * current_move;
                if (move_twice < 0)
                {
                    continue;
                }
                long total_moves = move_twice + prev_move;
                if (total_moves < 0)
                {
                    continue;
                }
                // Update if a better solution is found
                if (n_hanoi[twr][dsks] > total_moves || n_hanoi[twr][dsks] == -1)
                {
                    n_hanoi[twr][dsks] = total_moves;
                    k_hanoi[twr][dsks] = k;
                }
            }
        }
    }
}

// Fill moves so that each element is a two-integer VI describing the move
// You may assume that, initially, aux[] = {0, 1, 2, ..., n_twrs - 1}
void hanoi(vector<VI> &moves, int n_twrs, int n_dsks, VI &aux)
{
    // Base case: If only one disk, move it to the destination
    if (n_dsks == 1)
    {
        moves.push_back({aux[0], aux[1]});
        return;
    }

    // Move k disks to a spare tower
    swap(aux[1], aux[n_twrs - 1]);
    hanoi(moves, n_twrs, k_hanoi[n_twrs][n_dsks], aux);

    // Move remaining disks to the destination tower
    swap(aux[1], aux[n_twrs - 1]);
    hanoi(moves, n_twrs - 1, n_dsks - k_hanoi[n_twrs][n_dsks], aux);

    // Move k disks to the final destination
    swap(aux[0], aux[n_twrs - 1]);
    hanoi(moves, n_twrs, k_hanoi[n_twrs][n_dsks], aux);
    swap(aux[0], aux[n_twrs - 1]);
}
