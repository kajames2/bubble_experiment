#include "market_submission_result.hh"

#include <istream>
#include <ostream>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, MarketSubmissionResult res) {
  os << res.offer << " " << static_cast<bool>(res.trade);
  if (res.trade) {
    os << " " << *res.trade;
  }
  return os;
}

std::istream& operator>>(std::istream& is, MarketSubmissionResult& res) {
  bool has_trade;
  is >> res.offer >> has_trade;
  if (has_trade) {
    Trade trade;
    is >> trade;
    res.trade = trade;
  }
  return is;
}
}  // namespace assetmarket
