#ifndef TRADE_HH
#define TRADE_HH

#include "offer.hh"

namespace assetmarket {
struct Trade {
  unsigned int buyer;
  unsigned int seller;
  unsigned int price;
  unsigned int timestamp;
  unsigned int bid_id;
  unsigned int ask_id;

  auto operator<=>(const Trade&) const = default;
};

std::ostream& operator<<(std::ostream& os, Trade trade);
std::istream& operator>>(std::istream& is, Trade& trade);

}  // namespace assetmarket
#endif  // TRADE_HH
