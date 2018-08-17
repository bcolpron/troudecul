#include "game/json.h"
#include "catch.hpp"

using namespace cards;

TEST_CASE("json card to json")
{
    CHECK(to_json(Card{two, clubs}) ==
        "{\"rank\": \"two\", \"suit\": \"clubs\"}");
    CHECK(to_json(Card{three, diamonds}) ==
        "{\"rank\": \"three\", \"suit\": \"diamonds\"}");
    CHECK(to_json(Card{four, hearts}) ==
        "{\"rank\": \"four\", \"suit\": \"hearts\"}");
    CHECK(to_json(Card{five, spades}) ==
        "{\"rank\": \"five\", \"suit\": \"spades\"}");
    CHECK(to_json(Card{six, clubs}) ==
        "{\"rank\": \"six\", \"suit\": \"clubs\"}");
    CHECK(to_json(Card{seven, diamonds}) ==
        "{\"rank\": \"seven\", \"suit\": \"diamonds\"}");
    CHECK(to_json(Card{eight, hearts}) ==
        "{\"rank\": \"eight\", \"suit\": \"hearts\"}");
    CHECK(to_json(Card{nine, spades}) ==
        "{\"rank\": \"nine\", \"suit\": \"spades\"}");
    CHECK(to_json(Card{ten, clubs}) ==
        "{\"rank\": \"ten\", \"suit\": \"clubs\"}");
    CHECK(to_json(Card{jack, diamonds}) ==
        "{\"rank\": \"jack\", \"suit\": \"diamonds\"}");
    CHECK(to_json(Card{queen, hearts}) ==
        "{\"rank\": \"queen\", \"suit\": \"hearts\"}");
    CHECK(to_json(Card{king, spades}) ==
        "{\"rank\": \"king\", \"suit\": \"spades\"}");
    CHECK(to_json(Card{ace, clubs}) ==
        "{\"rank\": \"ace\", \"suit\": \"clubs\"}");
    CHECK(to_json(Card{red_joker}) ==
        "{\"joker\": \"red\"}");
    CHECK(to_json(Card{white_joker}) ==
        "{\"joker\": \"white\"}");
}

TEST_CASE("json empty hand to json")
{
    CHECK(to_json(Hand{}) == "[]");
}

TEST_CASE("json hand to json")
{
   CHECK(to_json(Hand{Card{king, spades}, Card{ace, clubs}}) ==
        "[{\"rank\": \"king\", \"suit\": \"spades\"}, {\"rank\": \"ace\", \"suit\": \"clubs\"}]");

}
