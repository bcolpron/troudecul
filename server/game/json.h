#pragma once

#include "cards.h"
#include <string>

namespace cards
{

std::string to_json(const Card& c);
std::string to_json(const Hand& h);

}