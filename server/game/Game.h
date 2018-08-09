#pragma once

#include "cards.h"
#include "Player.h"
#include <optional>

class Game
{
public:
    template<class PlayerSeq>
    Game(PlayerSeq& players) {}

    PlayerId next_to_play();
    std::optional<cards::Hand> hand_to_beat();

    void play();
    void play(const PlayerId& id, const cards::Hand& cards);
    void pass(const PlayerId& id);
};
