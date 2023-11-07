#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

using namespace std;

const int MAX_DEPTH = 5;
enum HandType
{
    HIGH_CARD = 1,
    PAIR,
    TWO_PAIRS,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH
};

class Card
{
public:
    int rank;
    int suit;
    Card() : rank(0), suit(0) {}
    Card(int r, int s)
    {
        rank = r;
        suit = s;
    }
};

vector<Card> decode_and_sort_hand(int hand)
{
    vector<Card> cards(5);
    for (int i = 0; i < 5; ++i)
    {
        int card = hand % 52;
        cards[i] = Card(card / 4, card % 4);
        hand /= 52;
    }

    // Sort the cards by rank
    for (int i = 1; i < 5; ++i)
    {
        Card key = cards[i];
        int j = i - 1;
        while (j >= 0 && cards[j].rank > key.rank)
        {
            cards[j + 1] = cards[j];
            j = j - 1;
        }
        cards[j + 1] = key;
    }
    return cards;
}

int score_hand(vector<Card> &cards)
{
    // TODO check for pair first bc then cant be straight
    int mult_factor = 1000000;
    // Check for Ace-low straight (A-2-3-4-5)
    bool isAceLowStraight = (cards[0].rank == 0 && cards[1].rank == 1 && cards[2].rank == 2 && cards[3].rank == 3 && cards[4].rank == 12);

    // Check for straight or straight flush
    bool isStraight = isAceLowStraight || (cards[0].rank == cards[1].rank - 1 && cards[1].rank == cards[2].rank - 1 && cards[2].rank == cards[3].rank - 1 && cards[3].rank == cards[4].rank - 1);
    bool isFlush = (cards[0].suit == cards[1].suit && cards[1].suit == cards[2].suit && cards[2].suit == cards[3].suit && cards[3].suit == cards[4].suit);

    // Straight flush
    if (isStraight && isFlush)
    {
        return STRAIGHT_FLUSH * mult_factor + (isAceLowStraight ? 3 : cards[4].rank);
    }

    // Four of a kind
    if ((cards[0].rank == cards[1].rank && cards[1].rank == cards[2].rank && cards[2].rank == cards[3].rank) ||
        (cards[1].rank == cards[2].rank && cards[2].rank == cards[3].rank && cards[3].rank == cards[4].rank))
    {
        int fourRank = cards[1].rank;
        int kicker = (cards[0].rank == fourRank) ? cards[4].rank : cards[0].rank;
        return FOUR_OF_A_KIND * mult_factor + fourRank * 15 + kicker;
    }

    // Full house
    if ((cards[0].rank == cards[1].rank && cards[1].rank == cards[2].rank && cards[3].rank == cards[4].rank) ||
        (cards[0].rank == cards[1].rank && cards[2].rank == cards[3].rank && cards[3].rank == cards[4].rank))
    {
        int threeRank = cards[2].rank;
        int pairRank = (cards[0].rank == threeRank) ? cards[4].rank : cards[0].rank;
        return FULL_HOUSE * mult_factor + threeRank * 15 + pairRank;
    }

    // Flush
    if (isFlush)
    {
        int score = FLUSH * mult_factor;
        for (int i = 4; i >= 0; --i)
        {
            score += cards[i].rank * pow(15, i);
        }
        return score;
    }

    // Straight
    if (isStraight)
    {
        return STRAIGHT * mult_factor + (isAceLowStraight ? 3 : cards[4].rank);
    }

    // Three of a kind
    for (int i = 0; i < 3; ++i)
    {
        if (cards[i].rank == cards[i + 1].rank && cards[i + 1].rank == cards[i + 2].rank)
        {
            int threeRank = cards[i].rank;
            // Kickers are the remaining cards not part of the three of a kind
            int kicker1 = cards[(i + 3) % 5].rank;
            int kicker2 = cards[(i + 4) % 5].rank;
            if (kicker1 < kicker2)
            {
                swap(kicker1, kicker2); // Ensure kicker1 is the highest
            }
            return THREE_OF_A_KIND * mult_factor + threeRank * 225 + kicker1 * 15 + kicker2; // 225 = 15*15
        }
    }

    // Two pairs
    if (cards[0].rank == cards[1].rank && cards[2].rank == cards[3].rank)
    {
        return TWO_PAIRS * mult_factor + cards[1].rank * 225 + cards[3].rank * 15 + cards[4].rank;
    }
    else if (cards[0].rank == cards[1].rank && cards[3].rank == cards[4].rank)
    {
        return TWO_PAIRS * mult_factor + cards[1].rank * 225 + cards[4].rank * 15 + cards[2].rank;
    }
    else if (cards[1].rank == cards[2].rank && cards[3].rank == cards[4].rank)
    {
        return TWO_PAIRS * mult_factor + cards[2].rank * 225 + cards[4].rank * 15 + cards[0].rank;
    }

    // Pair
    for (int i = 0; i < 4; ++i)
    {
        if (cards[i].rank == cards[i + 1].rank)
        {
            int pairRank = cards[i].rank;
            // Kickers are the remaining cards not part of the pair
            int kickers[3];
            int index = 0;
            for (int j = 0; j < 5; ++j)
            {
                if (j != i && j != i + 1)
                {
                    kickers[index++] = cards[j].rank;
                }
            }
            // Sort kickers using bubble sort
            for (int k = 0; k < 2; ++k)
            {
                for (int l = 0; l < 2 - k; ++l)
                {
                    if (kickers[l] < kickers[l + 1])
                    {
                        swap(kickers[l], kickers[l + 1]);
                    }
                }
            }
            return PAIR * mult_factor + pairRank * 3375 + kickers[0] * 225 + kickers[1] * 15 + kickers[2]; // 3375 = 15*15*15
        }
    }

    // High card
    int score = cards[4].rank * pow(15, 4) + cards[3].rank * pow(15, 3) + cards[2].rank * pow(15, 2) + cards[1].rank * 15 + cards[0].rank;
    return score;
}

void merge(vector<int> &a, vector<int> &scores, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    vector<int> L(n1), R(n2);
    vector<int> LS(n1), RS(n2);

    // Copy data to temp vectors
    for (int i = 0; i < n1; i++)
    {
        L[i] = a[left + i];
        LS[i] = scores[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = a[mid + 1 + j];
        RS[j] = scores[mid + 1 + j];
    }

    // Merge the temp vectors back into the original vectors
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (LS[i] <= RS[j])
        {
            a[k] = L[i];
            scores[k] = LS[i];
            i++;
        }
        else
        {
            a[k] = R[j];
            scores[k] = RS[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements of L[], LS[]
    while (i < n1)
    {
        a[k] = L[i];
        scores[k] = LS[i];
        i++;
        k++;
    }

    // Copy any remaining elements of R[], RS[]
    while (j < n2)
    {
        a[k] = R[j];
        scores[k] = RS[j];
        j++;
        k++;
    }
}

// Recursive function to sort the vector using merge sort
void mergeSort(vector<int> &a, vector<int> &scores, int left, int right, int depth = 0)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        if (depth < MAX_DEPTH)
        {
            // Sort first and second halves in parallel
            thread leftThread(mergeSort, ref(a), ref(scores), left, mid, depth + 1);
            mergeSort(a, scores, mid + 1, right, depth + 1);
            leftThread.join();
        }
        else
        {
            // Perform a regular merge sort without threading
            mergeSort(a, scores, left, mid, depth + 1);
            mergeSort(a, scores, mid + 1, right, depth + 1);
        }

        // Merge the sorted halves
        merge(a, scores, left, mid, right);
    }
}

void poker_sort(vector<int> &a)
{
    vector<int> scores(a.size());
    for (int i = 0; i < a.size(); i++)
    {
        vector<Card> sorted_hand = decode_and_sort_hand(a[i]);
        int score = score_hand(sorted_hand);
        scores[i] = score;
    }
    mergeSort(a, scores, 0, a.size() - 1);
}
