#include "creation_result.hh"

#include <string>

namespace assetmarket {

std::string PrettyPrint(CreationValidity val) {
  switch (val) {
    case CreationValidity::Accepted:
      return "Created Share";
    case CreationValidity::RejectedMarketClosed:
      return "Rejected Creation: Market Closed";
    case CreationValidity::RejectedCannotCreate:
      return "Rejected Creation: Cannot Create";
    case CreationValidity::RejectedInsufficientCash:
      return "Rejected Creation: Insufficient Cash";
  }
}
std::ostream& operator<<(std::ostream& os, const CreationResult& cr) {
  return os << static_cast<int>(cr.validity) << " " << cr.id;
}

std::istream& operator>>(std::istream& is, CreationResult& cr) {
  int valid;
  is >> valid;
  cr.validity = static_cast<CreationValidity>(valid);
  is >> cr.id;
  return is;
}
}  // namespace assetmarket
