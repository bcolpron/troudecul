#pragma once

#include "cards.h"
#include "Player.h"
#include <optional>

class Game
{
public:
    template<class PlayerIdSeq>
    Game(const PlayerIdSeq& players)
    : players_(players.begin(), players.end()),
      first_player_(players_.begin()),
      current_player_(first_player_) {}

    const PlayerId& current_player() const;
    const std::optional<cards::Hand>& hand_to_beat() const { return trick_; }

    void play(const PlayerId& id, const cards::Hand& cards);
    void pass(const PlayerId& id);

private:
    void move_on_next_player();

    PlayerIds players_;
    std::optional<cards::Hand> trick_;
    PlayerIds::iterator first_player_;
    PlayerIds::iterator current_player_;
    PlayerIds::iterator last_played_;
};

void deal_cards(Players& players);

bool is_valid_play(const std::optional<cards::Hand>& table, const cards::Hand& play);