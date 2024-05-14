/*
    Data Structures 2023-2024 Lab 10: Discard

    @author Krish Arora
*/

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class Card
{
public:
    int rank;
    int suit;
    Card() : rank(0), suit(0) {}
    Card(int r, int s) : rank(r), suit(s) {}
};

vector<Card> decode_hand(vector<int> hand)
{
    vector<Card> cards;
    for (int i = 0; i < 5; i++)
    {
        cards.push_back(Card(hand[i] / 4, hand[i] % 4));
    }
    for (int i = 1; i < 5; i++)
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

vector<Card> find_discard(vector<Card> &cards)
{
    bool isAceLowStraight = (cards[0].rank == 0 && cards[1].rank == 1 && cards[2].rank == 2 && cards[3].rank == 3 && cards[4].rank == 12);
    bool isStraight = isAceLowStraight || (cards[0].rank == cards[1].rank - 1 && cards[1].rank == cards[2].rank - 1 &&
                                            cards[2].rank == cards[3].rank - 1 && cards[3].rank == cards[4].rank - 1);
    bool isFlush = (cards[0].suit == cards[1].suit && cards[1].suit == cards[2].suit && cards[2].suit == cards[3].suit && cards[3].suit == cards[4].suit);
    bool isRoyalFlush = isFlush && cards[0].rank == 8 && cards[1].rank == 9 && cards[2].rank == 10 && cards[3].rank == 11 && cards[4].rank == 12;
    bool isStraightFlush = isFlush && isStraight;
    bool isFullHouse = (cards[0].rank == cards[1].rank && cards[1].rank == cards[2].rank && cards[3].rank == cards[4].rank) ||
                        (cards[0].rank == cards[1].rank && cards[2].rank == cards[3].rank && cards[3].rank == cards[4].rank);
    bool isFourOfAKind = (cards[0].rank == cards[1].rank && cards[1].rank == cards[2].rank && cards[2].rank == cards[3].rank) ||
                        (cards[1].rank == cards[2].rank && cards[2].rank == cards[3].rank && cards[3].rank == cards[4].rank);
    // Discard nothing if royal flush, straight flush, full house, flush, straight, or four of a kind
    if (isRoyalFlush || isStraightFlush || isFullHouse || isFlush || isStraight || isFourOfAKind)
    {
        return vector<Card>();
    }

    // If three of a kind, discard the two kickers
    if (cards[0].rank == cards[1].rank && cards[1].rank == cards[2].rank)
    {
        return vector<Card>{cards[3], cards[4]};
    }
    if (cards[1].rank == cards[2].rank && cards[2].rank == cards[3].rank)
    {
        return vector<Card>{cards[0], cards[4]};
    }
    if (cards[2].rank == cards[3].rank && cards[3].rank == cards[4].rank)
    {
        return vector<Card>{cards[0], cards[1]};
    }

    // If two pairs, discard the kicker
    if (cards[0].rank == cards[1].rank && cards[2].rank == cards[3].rank)
    {
        return vector<Card>{cards[4]};
    }
    if (cards[0].rank == cards[1].rank && cards[3].rank == cards[4].rank)
    {
        return vector<Card>{cards[2]};
    }

    // If pair, discard the three cards not part of the pair
    if (cards[0].rank == cards[1].rank)
    {
        return vector<Card>{cards[2], cards[3], cards[4]};
    }
    if (cards[1].rank == cards[2].rank)
    {
        return vector<Card>{cards[0], cards[3], cards[4]};
    }
    if (cards[2].rank == cards[3].rank)
    {
        return vector<Card>{cards[0], cards[1], cards[4]};
    }
    if (cards[3].rank == cards[4].rank)
    {
        return vector<Card>{cards[0], cards[1], cards[2]};
    }

    // If high card, discard the three lowest cards
    return vector<Card>{cards[0], cards[1], cards[2]};
}

int main()
{
    vector<int> hand(5);
    for (int i = 0; i < 5; i++)
    {
        cin >> hand[i];
    }
    vector<Card> cards = decode_hand(hand);
    vector<Card> discard = find_discard(cards);
    cout << discard.size();
    for (int i = 0; i < discard.size(); i++)
    {
        cout << ' ' << discard[i].rank * 4 + discard[i].suit;
    }
    cout << '\n';

    return 0;
}
