// Deck.h
#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

class Deck {
public:
    Deck();

    void Shuffle();
    Card Draw();             // Pega uma carta
    bool IsEmpty() const;

private:
    std::vector<Card> cards;
    size_t currentIndex;
};

#endif
