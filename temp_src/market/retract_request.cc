#include "retract_request.hh"

#include <string>

namespace assetmarket {

std::string PrettyPrint(RetractValidity val) {
  switch (val) {
    case RetractValidity::Accepted:
      return "Offer Retracted";
    case RetractValidity::RejectedNonExistent:
      return "Failed to Retract: Non-Existent Offer";
    case RetractValidity::RejectedOwnerMismatch:
      return "Failed to Retract: Not your Offer";
    case RetractValidity::RejectedMarketClosed:
      return "Failed to Retract: Market Closed";
  }
}
std::ostream& operator<<(std::ostream& os, RetractValidity val) {
  return os << static_cast<int>(val);
}
std::istream& operator>>(std::istream& is, RetractValidity& val) {
  int valid;
  is >> valid;
  val = static_cast<RetractValidity>(valid);
  return is;
}

std::ostream& operator<<(std::ostream& os, const RetractRequest& rr) {
  return os << rr.id << " " << rr.offer_id;
}

std::istream& operator>>(std::istream& is, RetractRequest& rr) {
  is >> rr.id;
  is >> rr.offer_id;
  return is;
}

std::ostream& operator<<(std::ostream& os, const RetractResult& rr) {
  os << rr.validity << " " << rr.request;
}

std::istream& operator>>(std::istream& is, RetractResult& rr) {
  return is >> rr.validity >> rr.request;
}

}  // namespace assetmarket
