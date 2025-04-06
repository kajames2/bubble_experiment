#include "market_state.hh"

namespace assetmarket {
std::ostream& operator<<(std::ostream& os, const MarketState& ms) {
  os << ms.asks << " " << ms.bids << " ";
  os << ms.transactions.size();
  for (auto& t : ms.transactions) {
    os << " " << t;
  }
  return os;
}
std::istream& operator>>(std::istream& is, MarketState& ms) {
  is >> ms.asks >> ms.bids;
  int size;
  is >> size;
  for (int i = 0; i < size; ++i) {
    Trade t;
    is >> t;
    ms.transactions.push_back(t);
  }
  return is;
}
}  // namespace assetmarket
