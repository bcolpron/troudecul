#include "game/Game.h"
#include "catch.hpp"

TEST_CASE("Game initialization", "[game]")
{
    std::array<Player, 4> players;
    Game g(players);
    CHECK(g.current_player() == players[0]);
    CHECK_FALSE(g.hand_to_beat());
}
