#include "offer.hh"

#include <ostream>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const Offer& offer) {
  return os << offer.id << ", " << offer.player_id << ", " << offer.price
            << ", " << offer.timestamp;
}
}  // namespace assetmarket
