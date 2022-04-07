#ifndef MARKET_SUBMISSION_RESULT_MESSAGE_HH
#define MARKET_SUBMISSION_RESULT_MESSAGE_HH

#include <string>

#include "market_submission_result.hh"
#include "message.hh"

namespace assetmarket {

class MarketSubmissionResultMessage : public Message {
 public:
  MarketSubmissionResultMessage(MarketSubmissionResult res) {}
  auto Serialize() const -> std::string override { return ""; };
  auto Deserialize(const std::string&) -> void override{};
};

}  // namespace assetmarket

#endif  // MARKET_SUBMISSION_RESULT_MESSAGE_HH
