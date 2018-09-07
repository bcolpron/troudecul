#include "Game.h"

const PlayerId& Game::current_player() const
{
    return *current_player_;
}

void Game::move_on_next_player()
{
    auto next = current_player_;
    if (++next == players_.end()) next = players_.begin();

    if (next == first_player_) {
        cards_on_table_ = std::nullopt;
        current_player_ = last_played_;
    } else {
        current_player_ = next;
    }
}

void Game::play(const PlayerId& id, const cards::Hand& cards)
{
    if (id != *current_player_) throw std::logic_error("Not player's turn");

    cards_on_table_ = cards;
    last_played_ = current_player_;

    move_on_next_player();
}

void Game::pass(const PlayerId& id)
{
    if (!cards_on_table_) throw std::logic_error("first player cannot pass");

    move_on_next_player();
}

bool is_valid_play(const std::optional<cards::Hand>& table, const cards::Hand& play)
{
    // Validate play hand
    if (play.empty()) return false;
    if (std::find_if(++play.begin(), play.end(),
                     [&] (auto card) { return card.rank() != play.begin()->rank(); })
        != play.end())
    {
        return false;
    }

    // if play is valid and table is empty, then fine.
    if (!table) return true;

    // Then, make sure play cards number is valid against table
    if (table->size() != play.size()
        && !play.begin()->is_joker()
        && !(play.begin()->rank() == cards::two && table->size()-1 == play.size()))
    {
        return false;
    }

    // Finally, compare card strenghs
    return cards::StrengthCompare()(*(*table).begin(), *play.begin());
}

void deal_cards(Players& players)
{
    auto deck = cards::shuffled_deck();
    players[0].set_hand(cards::Hand(deck.begin(),    deck.begin()+14));
    players[1].set_hand(cards::Hand(deck.begin()+14, deck.begin()+28));
    players[2].set_hand(cards::Hand(deck.begin()+28, deck.begin()+41));
    players[3].set_hand(cards::Hand(deck.begin()+41, deck.end()));
}