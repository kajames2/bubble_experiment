#include "market_submission_result.hh"

#include <istream>
#include <ostream>
#include <string>

#include "retract_request.hh"

namespace assetmarket {

std::string PrettyPrint(OfferValidity val) {
  switch (val) {
    case OfferValidity::Accepted:
      return "Offer Accepted";
    case OfferValidity::RejectedNoTradeoff:
      return "Offer Rejected: No Tradeoff";
    case OfferValidity::RejectedInsufficientUnits:
      return "Offer Rejected: Insufficient Units";
    case OfferValidity::RejectedInsufficientCash:
      return "Offer Rejected: Insufficient Cash";
    case OfferValidity::RejectedNonImproving:
      return "Offer Rejected: Non-Improving";
    case OfferValidity::RejectedTradeWithSelf:
      return "Offer Rejected: Trade With Self";
    case OfferValidity::RejectedPriceOutOfRange:
      return "Offer Rejected: Price Out Of Range";
    case OfferValidity::RejectedMarketClosed:
      return "Offer Rejected: Market Closed";
  }
}

std::ostream& operator<<(std::ostream& os, const MarketSubmissionResult& res) {
  os << static_cast<int>(res.validity) << " ";
  os << res.offer << " " << static_cast<bool>(res.trade);
  if (res.trade) {
    os << " " << *res.trade;
  }
  os << " " << res.retracts.size();
  for (const auto& retract : res.retracts) {
    os << " " << retract;
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
  size_t n_retracts;
  is >> n_retracts;
  for (size_t i = 0; i < n_retracts; i++) {
    RetractResult ret;
    is >> ret;
    res.retracts.push_back(ret);
  }
  return is;
}
}  // namespace assetmarket
