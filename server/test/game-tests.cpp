#include "game/Game.h"
#include "catch.hpp"
#include <boost/function_output_iterator.hpp>
#include <range/v3/core.hpp>
#include <range/v3/view/transform.hpp>

using namespace cards;

namespace
{
    const auto pair_of_threes = Hand{Card{three, hearts}, Card{three, spades}};
    const auto pair_of_five = Hand{Card{five, hearts}, Card{five, spades}};
    const auto pair_of_jack = Hand{Card{jack, hearts}, Card{jack, spades}};
    const auto pair_of_aces = Hand{Card{ace, hearts}, Card{ace, spades}};
}

auto ids = [](auto&& players){return players|ranges::view::transform([](const auto& p){return p.id();});};

TEST_CASE("Game initialization", "[game]")
{
    std::array<Player, 4> players;
    Game g(ids(players));
    CHECK(g.current_player() == players[0].id());
    CHECK_FALSE(g.hand_to_beat());
}

TEST_CASE("Game play", "[game]")
{
    std::array<Player, 4> players;
    Game g(ids(players));

    SECTION("first player plays")
    {
        g.play(players[0].id(), pair_of_threes);
        CHECK(g.hand_to_beat() == pair_of_threes);
        CHECK(g.current_player() == players[1].id());
    }

    SECTION("all players play")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.play(players[3].id(), pair_of_aces);
        CHECK(g.current_player() == players[3].id());
        CHECK_FALSE(g.hand_to_beat());
    }

    SECTION("all players play 2")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.play(players[3].id(), pair_of_aces);

        g.play(players[3].id(), pair_of_aces);
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        CHECK(g.current_player() == players[2].id());
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
    Game g(ids(players));

    SECTION("first player cannot pass")
    {
        CHECK_THROWS_AS(g.pass(players[0].id()), std::logic_error);
    }

    SECTION("next player can pass")
    {
        g.play(players[0].id(), pair_of_threes);
        g.pass(players[1].id());
        CHECK(g.current_player() == players[2].id());
    }

    SECTION("all others players pass")
    {
        g.play(players[0].id(), pair_of_threes);
        g.pass(players[1].id());
        g.pass(players[2].id());
        g.pass(players[3].id());
        CHECK(g.current_player() == players[0].id());
        CHECK_FALSE(g.hand_to_beat());
    }

    SECTION("all but last player passes")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.pass(players[3].id());
        CHECK(g.current_player() == players[2].id());
        CHECK_FALSE(g.hand_to_beat());
    }
}

TEST_CASE("Game play_and_finishes", "[game]")
{
    std::array<Player, 4> players;
    Game g(ids(players));

    SECTION("initial state - no titles")
    {
        CHECK(g.final_titles() == PlayerIds{});
    }

    SECTION("first player to finish get first title")
    {
        g.play_and_finish(players[0].id(), pair_of_jack);
        CHECK(g.current_player() == players[1].id());
        CHECK(g.final_titles() == PlayerIds{players[0].id()});
    }

    SECTION("players who finished no longer play")
    {
        g.play_and_finish(players[0].id(), pair_of_jack);
        g.play(players[1].id(), pair_of_jack);
        g.play(players[2].id(), pair_of_jack);
        g.play(players[3].id(), pair_of_jack);
        CHECK(g.current_player() == players[3].id());
    }

    SECTION("player next to finished player is next to play even if he passes")
    {
        g.play_and_finish(players[0].id(), pair_of_jack);
        g.pass(players[1].id());
        g.pass(players[2].id());
        g.pass(players[3].id());
        CHECK(g.current_player() == players[1].id());
    }

    SECTION("player next to finished player is next to play even if he passes 2")
    {
        g.play(players[0].id(), pair_of_five);
        g.play_and_finish(players[1].id(), pair_of_jack);
        g.pass(players[2].id());
        g.pass(players[3].id());
        CHECK(g.current_player() == players[2].id());
        CHECK_FALSE(g.hand_to_beat());
    }

    SECTION("all players finish")
    {
        g.play_and_finish(players[0].id(), pair_of_threes);
        g.play_and_finish(players[1].id(), pair_of_five);
        g.play_and_finish(players[2].id(), pair_of_jack);
        g.play_and_finish(players[3].id(), pair_of_aces);
        CHECK_FALSE(g.hand_to_beat());
        CHECK_THROWS_AS(g.current_player(), std::logic_error);
    }

}

TEST_CASE("is_valid_play anything beats nothing", "[game][rules]")
{
    CHECK(is_valid_play(std::nullopt, pair_of_threes));
    CHECK(is_valid_play(std::nullopt, Hand{Card{white_joker}}));
}

TEST_CASE("is_valid_play basic", "[game][rules]")
{
    CHECK(is_valid_play(pair_of_threes, pair_of_jack));
    CHECK_FALSE(is_valid_play(pair_of_jack, pair_of_threes));
}

TEST_CASE("is_valid_play empty hand", "[game][rules]")
{
    CHECK_FALSE(is_valid_play(std::nullopt, Hand{}));
}

TEST_CASE("is_valid_play mixed cards", "[game][rules]")
{
    CHECK_FALSE(is_valid_play(std::nullopt, Hand{Card{three, spades}, Card{four, spades}}));
}

TEST_CASE("is_valid_play wrong number of cards", "[game][rules]")
{
    CHECK_FALSE(is_valid_play(pair_of_threes, Hand{Card{four, hearts}}));
    CHECK_FALSE(is_valid_play(pair_of_threes, Hand{Card{four, hearts}, Card{four, clubs}, Card{four, spades}, }));
}

TEST_CASE("is_valid_play joker beats anything", "[game][rules]")
{
    CHECK(is_valid_play(pair_of_threes, Hand{Card{red_joker}}));
    // but not stronger joker
    CHECK_FALSE(is_valid_play(Hand{Card{white_joker}}, Hand{Card{red_joker}}));
}

TEST_CASE("is_valid_play one fewer two is required", "[game][rules]")
{
    CHECK(is_valid_play(pair_of_threes, Hand{Card{two, clubs}}));
    // still, can't be a pair of twos!
    CHECK_FALSE(is_valid_play(Hand{Card{two, spades}, Card{two, clubs}}, Hand{Card{two, hearts}}));
}

TEST_CASE("deal_cards", "[game]")
{
    Players players(4);
    deal_cards(players);
    CHECK(players[0].hand().size() == 14);
    CHECK(players[1].hand().size() == 14);
    CHECK(players[2].hand().size() == 13);
    CHECK(players[3].hand().size() == 13);

    auto count_common_cards = [] (auto hand1, auto hand2)
    {
        auto common_cards = 0;
        std::set_intersection(
            hand1.begin(), hand1.end(),
            hand2.begin(), hand2.end(),
            boost::make_function_output_iterator([&common_cards] (auto) {++common_cards;}),
            cards::OrderCompare());
        return common_cards;
    };

    CHECK(0 == count_common_cards(players[0].hand(), players[1].hand()));
    CHECK(0 == count_common_cards(players[1].hand(), players[2].hand()));
    CHECK(0 == count_common_cards(players[2].hand(), players[3].hand()));
    CHECK(0 == count_common_cards(players[3].hand(), players[0].hand()));
}