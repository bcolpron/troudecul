#pragma once

#include "cards.h"
#include "Player.h"
#include <optional>
#include <list>

class Round
{
public:
    template<class PlayerIdSeq>
    Round(const PlayerIdSeq& players)
    : players_(players.begin(), players.end()),
      first_player_(players_.begin()),
      current_player_(first_player_) {}

    const PlayerId& current_player() const;
    const std::optional<cards::Hand>& hand_to_beat() const { return trick_; }

    void play(const PlayerId& id, const cards::Hand& cards);
    void play_and_finish(const PlayerId& id, const cards::Hand& cards);
    void pass(const PlayerId& id);

    const PlayerIds& final_ranks()const { return final_ranks_; }
private:
    void move_on_next_player();

    using Players = std::list<PlayerId>;
    Players players_;
    Players::iterator first_player_;
    Players::iterator current_player_;
    Players::iterator last_played_;
    
    std::optional<cards::Hand> trick_;
    PlayerIds final_ranks_;
};

void deal_cards(Players& players);

bool is_valid_play(const std::optional<cards::Hand>& table, const cards::Hand& play);