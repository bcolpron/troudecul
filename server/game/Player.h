#pragma once

#include "cards.h"
#include <string>

using PlayerId = std::string;

class Player
{
public:
    Player();

    const PlayerId& id() const { return id_; }

    void set_hand(cards::Hand h) { hand_ = std::move(h); }
    const cards::Hand& hand() const { return hand_; }

    bool has(const cards::Hand& h) const { return includes(hand_, h); }
    void take(const cards::Hand& h);

private:
    std::string id_;
    cards::Hand hand_;
};

