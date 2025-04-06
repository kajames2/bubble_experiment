#ifndef MARKET_SUBMISSION_RESULT_HH
#define MARKET_SUBMISSION_RESULT_HH

#include <optional>
#include <string>
#include <vector>

#include "offer.hh"
#include "retract_request.hh"
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
  std::optional<Trade> trade = std::nullopt;
  std::vector<RetractResult> retracts = {};
  auto operator<=>(const MarketSubmissionResult&) const = default;
};

std::string PrettyPrint(OfferValidity val);
std::ostream& operator<<(std::ostream& os, const MarketSubmissionResult& res);
std::istream& operator>>(std::istream& is, MarketSubmissionResult& res);

}  // namespace assetmarket
#endif  // MARKET_SUBMISSION_RESULT_HH
