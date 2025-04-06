#ifndef MARKET_SUBMISSION_RESULT_HH
#define MARKET_SUBMISSION_RESULT_HH

#include <optional>
#include <sstream>
#include <string>

#include "offer.hh"
#include "trade.hh"

namespace assetmarket {

struct MarketSubmissionResult {
  Offer offer;
  std::optional<Trade> trade;
  auto operator<=>(const MarketSubmissionResult&) const = default;
};

std::ostream& operator<<(std::ostream& os, MarketSubmissionResult res);
std::istream& operator>>(std::istream& is, MarketSubmissionResult& res);

template <typename T>
std::string to_string(const T& value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

template <typename T>
void from_string(const std::string& str, T& value) {
  std::istringstream ss(str);
  ss >> value;
}

}  // namespace assetmarket
#endif  // MARKET_SUBMISSION_RESULT_HH
