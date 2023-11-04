#include <iostream>
#include <vector>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;

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

void insertion_sort(std::vector<Card> &cards)
{
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
}

int score_hand(std::vector<Card> &cards)
{
    int mult_factor = 100000;
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
            score += cards[i].rank * std::pow(15, i);
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
                std::swap(kicker1, kicker2);                                                 // Ensure kicker1 is the highest
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
                        std::swap(kickers[l], kickers[l + 1]);
                    }
                }
            }
            return PAIR * mult_factor + pairRank * 3375 + kickers[0] * 225 + kickers[1] * 15 + kickers[2]; // 3375 = 15*15*15
        }
    }

    // High card
    int score = HIGH_CARD * mult_factor;
    for (int i = 4; i >= 0; --i)
    {
        score += cards[i].rank * std::pow(15, i);
    }
    return score;
}

// Function to decode a hand and sort the cards by rank using insertion sort
std::vector<Card> decode_and_sort_hand(int hand)
{
    std::vector<Card> cards(5);
    for (int i = 0; i < 5; ++i)
    {
        int card = hand % 52;
        cards[i] = Card(card / 4, card % 4);
        hand /= 52;
    }

    // Sort the cards by rank
    insertion_sort(cards);

    return cards;
}

void poker_sort(vector<int> &a)
{
    vector<int> scores(a.size());
    for (int i = 0; i < a.size(); i++)
    {
        vector<Card> sorted_hand = decode_and_sort_hand(a[i]);
        scores[i] = score_hand(sorted_hand);
    }
    // sort a by scores
    for (int i = 1; i < a.size(); ++i)
    {
        int keyScore = scores[i];
        int keyHand = a[i];
        int j = i - 1;
        // Move elements of a[0..i-1] and scores[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && scores[j] > keyScore)
        {
            scores[j + 1] = scores[j];
            a[j + 1] = a[j];
            j = j - 1;
        }
        scores[j + 1] = keyScore;
        a[j + 1] = keyHand;
    }
}

int main()
{
    vector<int> a = {
        312524498,
        355399072,
        328369252,
        349203879,
        357681668,
        354759444,
        357117544,
        354803132,
        357107992,
        89302404};
    poker_sort(a);
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
    return 0;
}
