#include "market_submission_result.hh"

#include <istream>
#include <ostream>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const MarketSubmissionResult& res) {
  os << static_cast<int>(res.validity) << ";";
  os << res.offer << ";" << static_cast<bool>(res.trade);
  if (res.trade) {
    os << ";" << *res.trade;
  }
  return os;
}

std::istream& operator>>(std::istream& is, MarketSubmissionResult& res) {
  bool has_trade;
  int valid_num;
  is >> valid_num;
  res.validity = static_cast<OfferValidity>(valid_num);
  is >> res.offer >> has_trade;
  if (has_trade) {
    Trade trade;
    is >> trade;
    res.trade = trade;
  }
  return is;
}
}  // namespace assetmarket
