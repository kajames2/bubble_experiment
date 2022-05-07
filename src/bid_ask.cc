#include "bid_ask.hh"

namespace assetmarket {
std::ostream& operator<<(std::ostream& os, const BidAsk& ba) {
  return os << ba.id << ";" << ba.price;
}

std::istream& operator>>(std::istream& is, BidAsk& ba) {
  is >> ba.id;
  is >> ba.price;
  return is;
}

}  // namespace assetmarket
