// Deck.h
#ifndef POKERHAND_H
#define POKERHAND_H

#include "Card.h"
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>

class PokerHand {
public:
    enum HandRank {
        HIGH_CARD, ONE_PAIR, TWO_PAIRS, THREE_OF_A_KIND,
        STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND,
        STRAIGHT_FLUSH, ROYAL_FLUSH
    };

    PokerHand(const std::vector<Card>& cards);
    static HandRank Evaluate(const PokerHand& hand);
    const std::vector<Card>& GetCards() const;
    static std::string ToString(HandRank rank);
    static int PokerHandToDamage(HandRank rank);

private:
    std::vector<Card> cards;
};

#endif
