#ifndef RETRACT_REQUEST_HH
#define RETRACT_REQUEST_HH

#include <istream>
#include <ostream>
#include <string>

#include "subject.hh"

namespace assetmarket {
struct RetractRequest {
  SubjectID id;
  unsigned int offer_id;
};

std::ostream& operator<<(std::ostream& os, const RetractRequest& rr);
std::istream& operator>>(std::istream& is, RetractRequest& rr);

enum class RetractValidity {
  Accepted,
  RejectedNonExistent,
  RejectedOwnerMismatch,
  RejectedMarketClosed
};
struct RetractResult {
  RetractValidity validity;
  RetractRequest request;
};

std::string PrettyPrint(RetractValidity val);
std::ostream& operator<<(std::ostream& os, RetractValidity val);
std::istream& operator>>(std::istream& is, RetractValidity& val);

std::ostream& operator<<(std::ostream& os, const RetractResult& rr);
std::istream& operator>>(std::istream& is, RetractResult& rr);

}  // namespace assetmarket
#endif  // RETRACT_REQUEST_HH
