#ifndef SERIALIZABLE_MARKET_SUBMISSION_RESULT_HH
#define SERIALIZABLE_MARKET_SUBMISSION_RESULT_HH

#include <optional>

#include "market_submission_result.hh"
#include "message.hh"

namespace assetmarket {

struct SerializableMarketSubmissionResult : public Message {
  SerializableMarketSubmissionResult() {}
  virtual auto Serialize() const -> std::string { return ""; }
  virtual auto Deserialize(std::string) -> void{};
};
}  // namespace assetmarket
#endif  // SERIALIZABLE_MARKET_SUBMISSION_RESULT_HH
