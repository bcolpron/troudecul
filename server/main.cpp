
#include "game/Game.h"
#include "http/websockets.h"
#include <array>

int main(int, const char*[])
{
    std::array<Player, 4> players;
    Game game(players);

    WebSocketServer server;
    server.run(8888);

    return 0;
}

