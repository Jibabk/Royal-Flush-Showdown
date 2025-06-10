// Card.h
#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
    enum Suit { COPAS, OURO, PAUS, ESPADILHA };
    enum Rank {
        TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN,
        EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
    };

    Card(Suit suit, Rank rank);

    Suit GetSuit() const;
    Rank GetRank() const;

    std::string ToString() const;
    std::string GetSuitSymbol() const;
    std::string GetRankString() const;
    std::string GetImagePath() const;

private:
    Suit suit;
    Rank rank;
};

#endif
