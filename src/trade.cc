#include "trade.hh"

#include <istream>
#include <ostream>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
  return os << trade.buyer << " " << trade.seller << " " << trade.price << " "
            << trade.timestamp << " " << trade.bid_id << " " << trade.ask_id;
}

std::istream& operator>>(std::istream& is, Trade& trade) {
  return is >> trade.buyer >> trade.seller >> trade.price >> trade.timestamp >>
         trade.bid_id >> trade.ask_id;
}
}  // namespace assetmarket
