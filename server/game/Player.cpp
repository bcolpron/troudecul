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

const PlayerId& Player::id() const {
    return id_;
}