#include "Player.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

namespace
{
std::string generate_uuid()
{
    return boost::uuids::to_string(boost::uuids::random_generator()());
}
}

Player::Player()
: id_(generate_uuid()) {}

void Player::take(const cards::Hand& h)
{
    if (!includes(hand_, h)) throw std::logic_error("cannot take missing cards");
    std::for_each(h.begin(), h.end(), [this] (auto card) {
        hand_.erase(card);
    });
}