#pragma once

#include "Round.h"

struct RoundState
{
    cards::Hand trick;
    PlayerId current_player;
};


class RoundEventSink
{
public:

    virtual void broadcast(const RoundState& s) = 0;

private:
};
