#pragma once

#include <vector>
#include <cassert>
#include <stdexcept>
#include <random>
#include <algorithm>

namespace cards
{
    
enum Ranks { two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
enum Suits { clubs, diamonds, hearts, spades };
enum Jokers { red_joker=1, white_joker };

struct Card
{
    explicit Card(Ranks r, Suits s);
    explicit Card(Jokers j);

    Ranks rank() const;
    Suits suit() const;

    Jokers joker_type() const;

    bool is_regular() const;
    bool is_joker() const;

private:
    int data;

    friend bool operator==(const Card& c1, const Card& c2);
    friend bool operator!=(const Card& c1, const Card& c2);
};

bool operator==(const Card& c1, const Card& c2);
bool operator!=(const Card& c1, const Card& c2);

using Hand = std::vector<Card>;

std::vector<Card> shuffled_deck();

struct CardCompare
{
    /// returns true if lhs is weaker than rhs; false otherwise
    bool operator() (const Card& lhs, const Card& rhs) const;
};

// Implementation

inline Card::Card(Ranks r, Suits s)
: data((s << 4) + r) {}

inline Card::Card(Jokers j)
: data(j << 8) {}

inline Ranks Card::rank() const {
    if (is_joker()) throw std::logic_error("cannot access rank on Joker");
    auto r = data & 0b1111;
    assert (r >= two && r <= ace);
    return static_cast<Ranks>(r);
}

inline Suits Card::suit() const {
    if (is_joker()) throw std::logic_error("cannot access suit on Joker");
    auto s = data >> 4;
    assert (s >= clubs && s <= spades);
    return static_cast<Suits>(s);
}

inline Jokers Card::joker_type() const {
    if (is_regular()) throw std::logic_error("cannot access joker type on regular card");
    auto j = data >> 8;
    assert (j >= red_joker && j <= white_joker);
    return static_cast<Jokers>(j);
}

inline bool Card::is_regular() const {
    return data >> 8 == 0;
}

inline bool Card::is_joker() const {
    return data >> 8 != 0;
}

inline std::vector<Card> shuffled_deck() {
    auto d = std::vector<Card>{
        Card{two, clubs},    Card{three, clubs},    Card{four, clubs},    Card{five, clubs},    Card{six, clubs},    Card{seven, clubs},    Card{eight, clubs},    Card{nine, clubs},    Card{ten, clubs},    Card{jack, clubs},    Card{queen, clubs},    Card{king, clubs},    Card{ace, clubs},
        Card{two, diamonds}, Card{three, diamonds}, Card{four, diamonds}, Card{five, diamonds}, Card{six, diamonds}, Card{seven, diamonds}, Card{eight, diamonds}, Card{nine, diamonds}, Card{ten, diamonds}, Card{jack, diamonds}, Card{queen, diamonds}, Card{king, diamonds}, Card{ace, diamonds}, 
        Card{two, hearts},   Card{three, hearts},   Card{four, hearts},   Card{five, hearts},   Card{six, hearts},   Card{seven, hearts},   Card{eight, hearts},   Card{nine, hearts},   Card{ten, hearts},   Card{jack, hearts},   Card{queen, hearts},   Card{king, hearts},   Card{ace, hearts},
        Card{two, spades},   Card{three, spades},   Card{four, spades},   Card{five, spades},   Card{six, spades},   Card{seven, spades},   Card{eight, spades},   Card{nine, spades},   Card{ten, spades},   Card{jack, spades},   Card{queen, spades},   Card{king, spades},   Card{ace, spades},
        Card{red_joker},     Card{white_joker}
    };
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(begin(d), end(d), g);
    return d;
}

inline bool operator==(const Card& c1, const Card& c2) { return c1.data == c2.data; }
inline bool operator!=(const Card& c1, const Card& c2) { return c1.data != c2.data; }

inline bool CardCompare::operator() (const Card& lhs, const Card& rhs) const {
    if (lhs.is_joker()) {
        if (rhs.is_joker()) return lhs.joker_type() < rhs.joker_type();
        return false;
    }
    if (rhs.is_joker()) return true;
    if (lhs.rank() == two) return false;
    if (rhs.rank() == two) return true;
    return lhs.rank() < rhs.rank();
}
}