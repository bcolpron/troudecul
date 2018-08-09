#include "game/Player.h"
#include "catch.hpp"

TEST_CASE("Default construction and Identity", "[player]")
{
    Player p;
    CHECK(p.id().size() != 0);
}

TEST_CASE("Player hand", "[player]")
{
    using namespace cards;
    Player p;
    auto hand = Hand{Card{five, spades}, Card{king, clubs}};

    p.set_hand(hand);
    CHECK(hand == p.hand());
}

TEST_CASE("Player has cards", "[player]")
{
    using namespace cards;
    Player p;
    auto hand = Hand{Card{five, spades}, Card{five, hearts}, Card{king, clubs}};

    p.set_hand(hand);
    CHECK(p.has(Hand{Card{five, hearts}}));
}

TEST_CASE("Player has cards 2", "[player]")
{
    using namespace cards;
    Player p;
    auto hand = Hand{Card{five, spades}, Card{five, hearts}, Card{king, clubs}};

    p.set_hand(hand);
    CHECK_FALSE(p.has(Hand{Card{five, diamonds}}));
}

TEST_CASE("Player take cards", "[player]")
{
    using namespace cards;
    Player p;
    auto hand = Hand{Card{five, spades}, Card{five, hearts}, Card{king, clubs}};

    p.set_hand(hand);
    p.take(Hand{Card{five, hearts}, Card{five, spades}});

    CHECK(p.hand().size() == 1);
}

TEST_CASE("Player take cards - cannot take non present cards", "[player]")
{
    using namespace cards;
    Player p;
    auto hand = Hand{Card{five, spades}, Card{five, hearts}, Card{king, clubs}};

    p.set_hand(hand);

    CHECK_THROWS_AS(p.take(Hand{Card{five, diamonds}}), std::logic_error);
}
