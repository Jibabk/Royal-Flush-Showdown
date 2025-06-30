// Deck.cpp
#include "Deck.h"
#include <stdexcept>

Deck::Deck() : currentIndex(0) {
    for (int s = 0; s < 4; ++s) {
        for (int r = 2; r <= 14; ++r) {
            cards.emplace_back(static_cast<Card::Suit>(s), static_cast<Card::Rank>(r));
        }
    }
    Shuffle();
}

void Deck::Shuffle() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(std::rand()));
    currentIndex = 0;
}

Card Deck::Draw() {
    if (IsEmpty()) throw std::runtime_error("Deck is empty!");
    return cards[currentIndex++];
}

bool Deck::IsEmpty() const {
    return currentIndex >= cards.size();
}
