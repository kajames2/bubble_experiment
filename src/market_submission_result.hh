#ifndef MARKET_SUBMISSION_RESULT_HH
#define MARKET_SUBMISSION_RESULT_HH

#include <optional>

#include "offer.hh"
#include "trade.hh"

namespace assetmarket {

enum class OfferValidity {
  Accepted,
  RejectedNoTradeoff,
  RejectedInsufficientUnits,
  RejectedInsufficientCash,
  RejectedNonImproving,
  RejectedTradeWithSelf,
  RejectedPriceOutOfRange,
  RejectedMarketClosed
};
struct MarketSubmissionResult {
  OfferValidity validity;
  Offer offer;
  std::optional<Trade> trade;
  auto operator<=>(const MarketSubmissionResult&) const = default;
};

std::ostream& operator<<(std::ostream& os, const MarketSubmissionResult& res);
std::istream& operator>>(std::istream& is, MarketSubmissionResult& res);

}  // namespace assetmarket
#endif  // MARKET_SUBMISSION_RESULT_HH
