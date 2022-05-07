#include "creation_result.hh"

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const CreationResult& cr) {
  return os << static_cast<int>(cr.validity) << ";" << cr.id;
}

std::istream& operator>>(std::istream& is, CreationResult& cr) {
  int valid;
  is >> valid;
  cr.validity = static_cast<CreationValidity>(valid);
  is >> cr.id;
  return is;
}
}  // namespace assetmarket
