#include "game/Player.h"
#include "catch.hpp"

TEST_CASE("Default construction and Identity", "[player]")
{
    Player p;
    CHECK(p.id().size() != 0);
}
