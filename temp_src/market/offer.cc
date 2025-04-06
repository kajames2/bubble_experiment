#include "offer.hh"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace assetmarket {

std::string PrettyPrint(const Offer& offer) {
  std::stringstream ss;
  ss << offer.id << "\t" << offer.player_id << "\t" << offer.price << "\t"
     << offer.time_elapsed;
  return ss.str();
}
std::ostream& operator<<(std::ostream& os, const Offer& offer) {
  return os << offer.id << " " << offer.player_id << " " << offer.price << " "
            << offer.time_elapsed;
}

std::istream& operator>>(std::istream& is, Offer& offer) {
  return is >> offer.id >> offer.player_id >> offer.price >> offer.time_elapsed;
}
}  // namespace assetmarket
