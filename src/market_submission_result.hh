#ifndef MARKET_SUBMISSION_RESULT_HH
#define MARKET_SUBMISSION_RESULT_HH

#include <optional>

#include "offer.hh"
#include "trade.hh"

namespace assetmarket {
enum class MarketSubmissionStatus { Accepted, InvalidPlayer };

struct MarketSubmissionResult {
  MarketSubmissionStatus status;
  std::optional<Offer> offer;
  std::optional<Trade> trade;
};
}  // namespace assetmarket
#endif  // MARKET_SUBMISSION_RESULT_HH
