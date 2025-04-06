#ifndef OFFER_HH
#define OFFER_HH

#include <compare>
#include <ostream>
#include <string>
#include <tuple>

#include "subject.hh"

namespace exptemplate {
namespace market {
class DifferentOffersWithSameID : std::exception {};

struct Offer {
  unsigned int id;
  SubjectID player_id;
  int price;
  unsigned int time_elapsed;

  std::tuple<int, int> tie() const {
    int neg_time = -static_cast<int>(time_elapsed);
    return std::tuple(price, neg_time);
  }

  auto operator<=>(const Offer& other) const { return tie() <=> other.tie(); }
  bool operator==(const Offer& other) const { return tie() == other.tie(); }
};

std::string PrettyPrint(const Offer& offer);
std::ostream& operator<<(std::ostream& os, const Offer& offer);
std::istream& operator>>(std::istream& is, Offer& offer);
}  // namespace market
}  // namespace exptemplate
#endif  // OFFER_HH
