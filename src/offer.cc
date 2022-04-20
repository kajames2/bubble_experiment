#include "offer.hh"

#include <istream>
#include <ostream>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const Offer& offer) {
  return os << offer.id << " " << offer.player_id << " " << offer.price << " "
            << offer.timestamp;
}

std::istream& operator>>(std::istream& is, Offer& offer) {
  return is >> offer.id >> offer.player_id >> offer.price >> offer.timestamp;
}
}  // namespace assetmarket
