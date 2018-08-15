#include "game/Game.h"
#include "catch.hpp"

using namespace cards;

namespace
{
    const auto pair_of_threes = Hand{Card{three, hearts}, Card{three, spades}};
    const auto pair_of_five = Hand{Card{five, hearts}, Card{five, spades}};
    const auto pair_of_jack = Hand{Card{jack, hearts}, Card{jack, spades}};
    const auto pair_of_aces = Hand{Card{ace, hearts}, Card{ace, spades}};
}

TEST_CASE("Game initialization", "[game]")
{
    std::array<Player, 4> players;
    Game g(players);
    CHECK(g.current_player() == players[0]);
    CHECK_FALSE(g.hand_to_beat());
}

TEST_CASE("Game play", "[game]")
{
    std::array<Player, 4> players;
    Game g(players);

    SECTION("first player plays")
    {
        g.play(players[0].id(), pair_of_threes);
        CHECK(g.hand_to_beat() == pair_of_threes);
        CHECK(g.current_player() == players[1]);
    }

    SECTION("all players play")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.play(players[3].id(), pair_of_aces);
        CHECK(g.current_player() == players[3]);
        CHECK_FALSE(g.hand_to_beat());
    }

    SECTION("wrong player")
    {
        CHECK_THROWS_AS(g.play(players[1].id(), pair_of_threes), std::logic_error);
    }
}

TEST_CASE("Game pass", "[game]")
{
    std::array<Player, 4> players;
    Game g(players);

    SECTION("first player cannot pass")
    {
        CHECK_THROWS_AS(g.pass(players[0].id()), std::logic_error);
    }

    SECTION("next player can pass")
    {
        g.play(players[0].id(), pair_of_threes);
        g.pass(players[1].id());
        CHECK(g.current_player() == players[2]);
    }

    SECTION("all others players pass")
    {
        g.play(players[0].id(), pair_of_threes);
        g.pass(players[1].id());
        g.pass(players[2].id());
        g.pass(players[3].id());
        CHECK(g.current_player() == players[0]);
        CHECK_FALSE(g.hand_to_beat());
    }

    SECTION("all but last player passes")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.pass(players[3].id());
        CHECK(g.current_player() == players[2]);
        CHECK_FALSE(g.hand_to_beat());
    }

}