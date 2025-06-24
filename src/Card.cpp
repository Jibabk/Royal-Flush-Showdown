// Card.cpp
#include "Card.h"

Card::Card(Suit suit, Rank rank) : suit(suit), rank(rank) {}
Card::Suit Card::GetSuit() const { return suit; }
Card::Rank Card::GetRank() const { return rank; }

std::string Card::GetSuitSymbol() const {
    switch (suit) {
        case COPAS: return "♥";
        case OURO: return "♦";
        case PAUS: return "♣";
        case ESPADILHA: return "♠";
        default: return "?";
    }
}

std::string Card::GetRankString() const {
    switch (rank) {
        case JACK: return "J";
        case QUEEN: return "Q";
        case KING: return "K";
        case ACE: return "A";
        default: return std::to_string(static_cast<int>(rank));
    }
}

std::string Card::ToString() const {
    return GetRankString() + GetSuitSymbol();
}

std::string Card::GetImagePath() const {
    std::string rankStr = GetRankString();
    std::string suitChar;

    switch (suit) {
        case COPAS:   suitChar = "C"; break;
        case OURO: suitChar = "O"; break;
        case PAUS:    suitChar = "P"; break;
        case ESPADILHA:   suitChar = "E"; break;
    }

    return "Recursos/img/cards/" + rankStr + suitChar + ".png";
}

