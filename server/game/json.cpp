#include "json.h"
#include <sstream>
#include <iterator>

namespace cards
{

std::string to_json(Ranks r)
{
    switch (r) {
        case two: return "\"two\"";
        case three: return "\"three\"";
        case four: return "\"four\"";
        case five: return "\"five\"";
        case six: return "\"six\"";
        case seven: return "\"seven\"";
        case eight: return "\"eight\"";
        case nine: return "\"nine\"";
        case ten: return "\"ten\"";
        case jack: return "\"jack\"";
        case queen: return "\"queen\"";
        case king: return "\"king\"";
        case ace: return "\"ace\"";
        default: assert("unexpected rank value" == 0);
    }
}

std::string to_json(Suits s)
{
    switch (s) {
        case clubs: return "\"clubs\"";
        case diamonds: return "\"diamonds\"";
        case hearts: return "\"hearts\"";
        case spades: return "\"spades\"";
        default: assert("unexpected suit value" == 0);
    }
}

std::string to_json(const Card& c)
{
    if (c.is_joker())
    {
        std::ostringstream s;
        s << "{\"joker\": " << (c.joker_type() == red_joker ? "\"red\"" : "\"white\"")
          << "}";
        return s.str();
    }
    else
    {
        std::ostringstream s;
        s << "{\"rank\": " << to_json(c.rank())
          << ", \"suit\": " << to_json(c.suit())
          << "}";
        return s.str();
    }
}

std::string to_json(const Hand& h)
{
    if (h.empty()) return "[]";
    std::ostringstream s;
    s << "[";
    s << to_json(*h.begin());
    std::for_each(++h.begin(), h.end(),
                  [&s] (const auto& card) {s << ", " << to_json(card);});
    s << "]";
    return s.str();
}

}