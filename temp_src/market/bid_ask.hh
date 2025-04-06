#ifndef BID_ASK_HH
#define BID_ASK_HH

#include <istream>
#include <ostream>

#include "subject.hh"

namespace assetmarket {
struct BidAsk {
  SubjectID id;
  int price;
};

std::ostream& operator<<(std::ostream& os, const BidAsk& ba);
std::istream& operator>>(std::istream& is, BidAsk& ba);

}  // namespace assetmarket
#endif  // BID_ASK_HH
