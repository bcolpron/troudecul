#include "Game.h"
//#include "http/websockets.h"

const Player& Game::current_player() const
{
    return players_[0];
}
