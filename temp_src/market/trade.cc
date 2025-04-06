#include "trade.hh"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace assetmarket {

std::string PrettyPrint(const Trade& t) {
  std::stringstream ss;
  ss << t.buyer << '\t' << t.seller << '\t' << t.price << '\t' << t.timestamp;
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
  return os << trade.buyer << " " << trade.seller << " " << trade.price << " "
            << trade.timestamp << " " << trade.bid_id << " " << trade.ask_id;
}

std::istream& operator>>(std::istream& is, Trade& trade) {
  return is >> trade.buyer >> trade.seller >> trade.price >> trade.timestamp >>
         trade.bid_id >> trade.ask_id;
}
}  // namespace assetmarket
