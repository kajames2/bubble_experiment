#ifndef TRADE_HH
#define TRADE_HH

#include <string>

#include "market/offer.hh"

namespace exptemplate {
namespace market {
struct Trade {
  SubjectID buyer;
  SubjectID seller;
  unsigned int price;
  unsigned int timestamp;
  unsigned int bid_id;
  unsigned int ask_id;

  auto operator<=>(const Trade&) const = default;
};

std::string PrettyPrint(const Trade& t);
std::ostream& operator<<(std::ostream& os, const Trade& trade);
std::istream& operator>>(std::istream& is, Trade& trade);
}  // namespace market
}  // namespace exptemplate
#endif  // TRADE_HH
