#include "PokerHand.h"
#include <stdexcept>

enum Suit { COPAS, OURO, PAUS, ESPADILHA };
enum Rank {
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN,
    EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

PokerHand::PokerHand(const std::vector<Card>& cards) : cards(cards) {}
const std::vector<Card>& PokerHand::GetCards() const { return cards; }


PokerHand::HandRank PokerHand::Evaluate(const PokerHand& hand) {
    const auto& cards = hand.GetCards();
    if (cards.size() != 5) return HIGH_CARD;

    std::map<int, int> rankCount;
    std::map<Suit, int> suitCount;

    std::vector<int> values;

    for (const Card& c : cards) {
        int val = static_cast<int>(c.GetRank());
        values.push_back(val);
        rankCount[val]++;
        suitCount[static_cast<Suit>(c.GetSuit())]++;
    }


    std::sort(values.begin(), values.end());
    bool isFlush = suitCount.size() == 1;
    bool isStraight = true;

    for (int i = 1; i < values.size(); ++i) {
        if (values[i] != values[i - 1] + 1) {
            isStraight = false;
            break;
        }
    }

    if (isFlush && isStraight && values.back() == static_cast<int>(Rank::ACE)) return ROYAL_FLUSH;
    if (isFlush && isStraight) return STRAIGHT_FLUSH;

    for (std::map<int, int>::const_iterator it = rankCount.begin(); it != rankCount.end(); ++it) {
        if (it->second == 4) return FOUR_OF_A_KIND;
    }


    bool three = false, pair = false;
    for (const auto& [rank, count] : rankCount) {
        if (count == 3) three = true;
        if (count == 2) pair = true;
    }

    if (three && pair) return FULL_HOUSE;
    if (isFlush) return FLUSH;
    if (isStraight) return STRAIGHT;
    if (three) return THREE_OF_A_KIND;

    int pairCount = 0;
    for (std::map<int, int>::const_iterator it = rankCount.begin(); it != rankCount.end(); ++it) {
        if (it->second == 2) {
            pairCount++;
        }
    }


    if (pairCount == 2) return TWO_PAIRS;
    if (pairCount == 1) return ONE_PAIR;

    return HIGH_CARD;
}

std::string PokerHand::ToString(PokerHand::HandRank rank) {
    switch (rank) {
        case ROYAL_FLUSH:     return "Royal Flush";
        case STRAIGHT_FLUSH:  return "Straight Flush";
        case FOUR_OF_A_KIND:  return "Four of a Kind";
        case FULL_HOUSE:      return "Full House";
        case FLUSH:           return "Flush";
        case STRAIGHT:        return "Straight";
        case THREE_OF_A_KIND: return "Three of a Kind";
        case TWO_PAIRS:       return "Two Pairs";
        case ONE_PAIR:        return "One Pair";
        case HIGH_CARD:       return "High Card";
        default:              return "Unknown";
    }
}

// Pode ser colocado em PokerHand.h ou em um utilitário
int PokerHand::PokerHandToDamage(PokerHand::HandRank rank) {
    switch (rank) {
        case PokerHand::ROYAL_FLUSH:     return 100;
        case PokerHand::STRAIGHT_FLUSH:  return 80;
        case PokerHand::FOUR_OF_A_KIND:  return 60;
        case PokerHand::FULL_HOUSE:      return 50;
        case PokerHand::FLUSH:           return 40;
        case PokerHand::STRAIGHT:        return 35;
        case PokerHand::THREE_OF_A_KIND: return 25;
        case PokerHand::TWO_PAIRS:       return 15;
        case PokerHand::ONE_PAIR:        return 10;
        case PokerHand::HIGH_CARD:       return 5;
        default:                         return 0;
    }
}