#pragma once

#include "cards.h"
#include "Player.h"
#include <optional>

class Game
{
public:
    template<class PlayerSeq>
    Game(PlayerSeq& players)
    : players_(begin(players), end(players)),
      first_player_(players_.begin()),
      current_player_(first_player_) {}

    const Player& current_player() const;
    const std::optional<cards::Hand>& hand_to_beat() const { return cards_on_table_; }

    void play(const PlayerId& id, const cards::Hand& cards);
    void pass(const PlayerId& id);

private:
    void move_on_next_player();

    Players players_;
    std::optional<cards::Hand> cards_on_table_;
    Players::iterator first_player_;
    Players::iterator current_player_;
    Players::iterator last_played_;
};

void deal_cards();

bool is_valid_play(const std::optional<cards::Hand>& table, const cards::Hand& play);