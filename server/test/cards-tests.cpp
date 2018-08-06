#include "game/Game.h"
#include "catch.hpp"
#include <stdexcept>

using namespace cards;

TEST_CASE("regular card", "[cards]")
{
    auto c = Card(two, clubs);
    CHECK(c.rank() == two);
    CHECK(c.suit() == clubs);
}

TEST_CASE("regular card 2", "[cards]")
{
    auto c = Card(ace, spades);
    CHECK(c.rank() == ace);
    CHECK(c.suit() == spades);
}

TEST_CASE("regular card detection", "[cards]")
{
    auto c = Card(five, diamonds);
    CHECK(c.is_regular());
    CHECK_FALSE(c.is_joker());
}

TEST_CASE("joker", "[cards]")
{
    auto c = Card(red_joker);
    CHECK(c.joker_type() == red_joker);
    CHECK(c.is_joker());
    CHECK_FALSE(c.is_regular());
}

TEST_CASE("rank access on joker", "[cards]")
{
    auto c = Card(white_joker);
    CHECK_THROWS_AS(c.rank(), std::logic_error);
}

TEST_CASE("suit access on joker", "[cards]")
{
    auto c = Card(white_joker);
    CHECK_THROWS_AS(c.suit(), std::logic_error);
}

TEST_CASE("joker type access on regular card", "[cards]")
{
    auto c = Card(five, diamonds);
    CHECK_THROWS_AS(c.joker_type(), std::logic_error);
}
