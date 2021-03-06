
#include "game/Round.h"
#include "http/websockets.h"
#include <range/v3/core.hpp>
#include <range/v3/view/transform.hpp>
#include <array>

int main(int, const char*[])
{
    std::array<Player, 4> players;
    Round game(ranges::view::transform(players, [] (const auto& player) {return player.id();}),
               nullptr);

    WebSocketServer server;
    server.run(8888);

    return 0;
}

