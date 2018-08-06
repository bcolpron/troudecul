#pragma once

#include <vector>
#include <cassert>
#include <stdexcept>

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
};

using Hands = std::vector<Card>;


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

}