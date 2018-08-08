#pragma once

#include "cards.h"

using PlayerId = std::string;

class Player
{
public:
    explicit Player();

    const PlayerId& id() const;

    void set_hand(cards::Hand);
    const cards::Hand& hand() const;

    void has(const cards::Hand&);
    void take(cards::Hand);
};

class Game
{
public:
    //Game(std::vector<PlayerId> players);

    PlayerId next_to_play();

    void play();
    void play(const PlayerId& id, const cards::Hand& cards);
    void pass(const PlayerId& id);
};
