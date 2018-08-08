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

TEST_CASE("card comparison", "[cards]")
{
    CHECK(Card{king, spades} == Card{king, spades});
    CHECK_FALSE(Card{king, spades} == Card{king, clubs});
    CHECK_FALSE(Card{king, spades} == Card{jack, spades});
    CHECK_FALSE(Card{king, spades} == Card{red_joker});
    CHECK_FALSE(Card{red_joker} == Card{white_joker});
    CHECK(Card{red_joker} == Card{red_joker});
}
TEST_CASE("card comparison 2", "[cards]")
{
    CHECK_FALSE(Card{king, spades} != Card{king, spades});
    CHECK(Card{king, spades} != Card{king, clubs});
    CHECK(Card{king, spades} != Card{jack, spades});
    CHECK(Card{king, spades} != Card{red_joker});
    CHECK(Card{red_joker} != Card{white_joker});
    CHECK_FALSE(Card{red_joker} != Card{red_joker});
}

TEST_CASE("deck", "[cards]")
{
    auto d = shuffled_deck();
    CHECK(d.size() == 54);
    CHECK(1 == std::count(begin(d), end(d), Card{king, spades}));
    CHECK(1 == std::count(begin(d), end(d), Card{queen, hearts}));
    CHECK(1 == std::count(begin(d), end(d), Card{red_joker}));
    // etc...
}

TEST_CASE("suffled_deck", "[cards]")
{
    // two shuffled deck should not be equal (unless really, really lucky)
    CHECK(shuffled_deck() != shuffled_deck());
}

TEST_CASE("CardCompare 1", "[cards]")
{
    CardCompare comp;
    CHECK(comp(Card{three, spades}, Card{five, hearts}));
    CHECK_FALSE(comp(Card{five, hearts}, Card{three, spades}));
    CHECK_FALSE(comp(Card{five, hearts}, Card{five, clubs}));
}

TEST_CASE("CardCompare 2", "[cards]")
{
    CardCompare comp;
    CHECK(comp(Card{king, spades}, Card{ace, hearts}));
    CHECK_FALSE(comp(Card{ace, hearts}, Card{king, spades}));
    CHECK_FALSE(comp(Card{ace, hearts}, Card{ace, clubs}));
}

TEST_CASE("CardCompare 3", "[cards]")
{
    CardCompare comp;
    CHECK(comp(Card{ace, spades}, Card{two, hearts}));
    CHECK_FALSE(comp(Card{two, hearts}, Card{ace, spades}));
    CHECK_FALSE(comp(Card{two, hearts}, Card{two, clubs}));
}

TEST_CASE("CardCompare 4", "[cards]")
{
    CardCompare comp;
    CHECK(comp(Card{two, spades}, Card{red_joker}));
    CHECK_FALSE(comp(Card{red_joker}, Card{two, spades}));
    CHECK_FALSE(comp(Card{red_joker}, Card{red_joker}));
}

TEST_CASE("CardCompare 5", "[cards]")
{
    CardCompare comp;
    CHECK(comp(Card{red_joker}, Card{white_joker}));
    CHECK_FALSE(comp(Card{white_joker}, Card{red_joker}));
    CHECK_FALSE(comp(Card{white_joker}, Card{white_joker}));
}
