#pragma once

#include "cards.h"
#include <string>

using PlayerId = std::string;

class Player
{
public:
    Player();

    const PlayerId& id() const;

    void set_hand(cards::Hand);
    const cards::Hand& hand() const;

    void has(const cards::Hand&);
    void take(cards::Hand);

private:
    std::string id_;
};

