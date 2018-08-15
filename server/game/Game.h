#pragma once

#include "cards.h"
#include "Player.h"
#include <optional>
#include <boost/container/small_vector.hpp>

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
    typedef boost::container::small_vector<Player, 4> Players;

    void move_on_next_player();

    Players players_;
    std::optional<cards::Hand> cards_on_table_;
    Players::iterator first_player_;
    Players::iterator current_player_;
    Players::iterator last_played_;
};
