#ifndef OFFER_HH
#define OFFER_HH

#include <compare>
#include <ostream>
#include <tuple>

namespace assetmarket {
class DifferentOffersWithSameID : std::exception {};

struct Offer {
  unsigned int id;
  unsigned int player_id;
  int price;
  unsigned int timestamp;

  std::tuple<int, int> tie() const {
    int neg_time = -static_cast<int>(timestamp);
    return std::tuple(price, neg_time);
  }

  auto operator<=>(const Offer& other) const { return tie() <=> other.tie(); }
  bool operator==(const Offer& other) const { return tie() == other.tie(); }
};

std::ostream& operator<<(std::ostream& os, const Offer& offer);
std::istream& operator>>(std::istream& is, Offer& offer);
};      // namespace assetmarket
#endif  // OFFER_HH
