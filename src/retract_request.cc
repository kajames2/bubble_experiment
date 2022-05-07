#include "retract_request.hh"

namespace assetmarket {
std::ostream& operator<<(std::ostream& os, const RetractRequest& rr) {
  return os << rr.id << ";" << rr.offer_id;
}

std::istream& operator>>(std::istream& is, RetractRequest& rr) {
  is >> rr.id;
  is >> rr.offer_id;
  return is;
}

std::ostream& operator<<(std::ostream& os, const RetractResult& rr) {
  os << static_cast<int>(rr.validity) << ";" << rr.request;
}

std::istream& operator>>(std::istream& is, RetractResult& rr) {
  int valid;
  is >> valid;
  rr.validity = static_cast<RetractValidity>(valid);
  is >> rr.request;
  return is;
}

}  // namespace assetmarket
