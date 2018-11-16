#include "game/Round.h"
#include "game/Events.h"
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

    struct SinkMock: RoundEventSink
    {
        virtual void broadcast(const RoundState& s) override
        {
            call_count++;
            last_state = s;
        }

        int call_count = 0;
        RoundState last_state;
    };

    auto make_sink() { return std::make_shared<SinkMock>(); }
}

auto ids = [](auto&& players){return players|ranges::view::transform([](const auto& p){return p.id();});};

TEST_CASE("Round initialization", "[Round]")
{
    std::array<Player, 4> players;
    Round g(ids(players), make_sink());
    CHECK(g.current_player() == players[0].id());
    CHECK(g.trick().empty());
}

TEST_CASE("Round play", "[Round]")
{
    std::array<Player, 4> players;
    Round g(ids(players), make_sink());

    SECTION("first player plays")
    {
        g.play(players[0].id(), pair_of_threes);
        CHECK(g.trick() == pair_of_threes);
        CHECK(g.current_player() == players[1].id());
    }

    SECTION("all players play")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.play(players[3].id(), pair_of_aces);
        CHECK(g.current_player() == players[3].id());
        CHECK(g.trick().empty());
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
        CHECK(g.trick().empty());
    }

    SECTION("wrong player")
    {
        CHECK_THROWS_AS(g.play(players[1].id(), pair_of_threes), std::logic_error);
    }
}

TEST_CASE("Round pass", "[Round]")
{
    std::array<Player, 4> players;
    Round g(ids(players), make_sink());

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
        CHECK(g.trick().empty());
    }

    SECTION("all but last player passes")
    {
        g.play(players[0].id(), pair_of_threes);
        g.play(players[1].id(), pair_of_five);
        g.play(players[2].id(), pair_of_jack);
        g.pass(players[3].id());
        CHECK(g.current_player() == players[2].id());
        CHECK(g.trick().empty());
    }
}

TEST_CASE("Round play_and_finishes", "[Round]")
{
    std::array<Player, 4> players;
    Round g(ids(players), make_sink());

    SECTION("initial state - no ranks")
    {
        CHECK(g.final_ranks() == PlayerIds{});
    }

    SECTION("first player to finish get first rank")
    {
        g.play_and_finish(players[0].id(), pair_of_jack);
        CHECK(g.current_player() == players[1].id());
        CHECK(g.final_ranks() == PlayerIds{players[0].id()});
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
        CHECK(g.trick().empty());
    }
}

TEST_CASE("round finished", "[Round]")
{
    std::array<Player, 4> players;
    Round g(ids(players), make_sink());

    // all players finish
    g.play_and_finish(players[0].id(), pair_of_threes);
    g.play_and_finish(players[1].id(), pair_of_five);
    g.play_and_finish(players[2].id(), pair_of_jack);
    g.play_and_finish(players[3].id(), pair_of_aces);

    SECTION("basic state validation")
    {
        CHECK(g.trick().empty());
        CHECK_THROWS_AS(g.current_player(), std::logic_error);
    }

    SECTION("play and pass")
    {
        CHECK_THROWS_AS(g.play(players[0].id(), pair_of_threes), std::logic_error);
        CHECK_THROWS_AS(g.play_and_finish(players[0].id(), pair_of_threes), std::logic_error);
        CHECK_THROWS_AS(g.pass(players[0].id()), std::logic_error);
    }

    SECTION("ranks")
    {
        CHECK(g.final_ranks() == PlayerIds{ids(players)});
    }
}

TEST_CASE("is_valid_play anything beats nothing", "[Round][rules]")
{
    CHECK(is_valid_play(Hand{}, pair_of_threes));
    CHECK(is_valid_play(Hand{}, Hand{Card{white_joker}}));
}

TEST_CASE("is_valid_play basic", "[Round][rules]")
{
    CHECK(is_valid_play(pair_of_threes, pair_of_jack));
    CHECK_FALSE(is_valid_play(pair_of_jack, pair_of_threes));
}

TEST_CASE("is_valid_play empty hand", "[Round][rules]")
{
    CHECK_FALSE(is_valid_play(Hand{}, Hand{}));
}

TEST_CASE("is_valid_play mixed cards", "[Round][rules]")
{
    CHECK_FALSE(is_valid_play(Hand{}, Hand{Card{three, spades}, Card{four, spades}}));
}

TEST_CASE("is_valid_play wrong number of cards", "[Round][rules]")
{
    CHECK_FALSE(is_valid_play(pair_of_threes, Hand{Card{four, hearts}}));
    CHECK_FALSE(is_valid_play(pair_of_threes, Hand{Card{four, hearts}, Card{four, clubs}, Card{four, spades}, }));
}

TEST_CASE("is_valid_play joker beats anything", "[Round][rules]")
{
    CHECK(is_valid_play(pair_of_threes, Hand{Card{red_joker}}));
    // but not stronger joker
    CHECK_FALSE(is_valid_play(Hand{Card{white_joker}}, Hand{Card{red_joker}}));
}

TEST_CASE("is_valid_play one fewer two is required", "[Round][rules]")
{
    CHECK(is_valid_play(pair_of_threes, Hand{Card{two, clubs}}));
    // still, can't be a pair of twos!
    CHECK_FALSE(is_valid_play(Hand{Card{two, spades}, Card{two, clubs}}, Hand{Card{two, hearts}}));
}

TEST_CASE("deal_cards", "[Round]")
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

TEST_CASE("eventing", "[Round]")
{
    std::array<Player, 4> players;
    auto sink = make_sink();
    Round g(ids(players), sink);

    SECTION("initial notification") 
    {
        CHECK(sink->call_count == 1);
        CHECK(sink->last_state.current_player == players[0].id());
        CHECK(sink->last_state.trick == Hand());
    }

    SECTION("after first player plays")
    {
        g.play(players[0].id(), pair_of_jack);
        CHECK(sink->call_count == 2);
        CHECK(sink->last_state.current_player == players[1].id());
        CHECK(sink->last_state.trick == pair_of_jack);
    }

    SECTION("after a player passes")
    {
        g.play(players[0].id(), pair_of_jack);
        g.pass(players[1].id());
        CHECK(sink->call_count == 3);
        CHECK(sink->last_state.current_player == players[2].id());
        CHECK(sink->last_state.trick == pair_of_jack);
    }

}