#pragma once

#include "cards.h"
#include "Player.h"
#include "Events.h"
#include <list>
#include <memory>

class Round
{
public:
    template<class PlayerIdSeq>
    Round(const PlayerIdSeq& players,
          std::shared_ptr<RoundEventSink> event_sink)
    : players_(players.begin(), players.end()),
      first_player_(players_.begin()),
      current_player_(first_player_),
      event_sink_{event_sink}
    {
        broadcast();
    }

    const PlayerId& current_player() const;
    const cards::Hand& trick() const { return trick_; }

    void play(const PlayerId& id, const cards::Hand& cards);
    void play_and_finish(const PlayerId& id, const cards::Hand& cards);
    void pass(const PlayerId& id);

    const PlayerIds& final_ranks()const { return final_ranks_; }
private:
    void move_on_next_player();
    void broadcast();

    using Players = std::list<PlayerId>;
    Players players_;
    Players::iterator first_player_;
    Players::iterator current_player_;
    Players::iterator last_played_;
    
    cards::Hand trick_;
    PlayerIds final_ranks_;

    std::shared_ptr<RoundEventSink> event_sink_;
};

void deal_cards(Players& players);

bool is_valid_play(const cards::Hand& trick, const cards::Hand& play);