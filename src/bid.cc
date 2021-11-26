#include "bid.hh"

[[nodiscard]] auto operator==(const Bid& b1, const Bid& b2) -> bool {
  return b1.tie() == b2.tie();
}
[[nodiscard]] auto operator<(const Bid& b1, const Bid& b2) -> bool {
  return b1.tie() < b2.tie();
}
[[nodiscard]] auto operator>(const Bid& b1, const Bid& b2) -> bool {
  return !((b1 < b2) || (b1 == b2));
}
[[nodiscard]] auto operator<=(const Bid& b1, const Bid& b2) -> bool {
  return (b1 < b2) || (b1 == b2);
}
[[nodiscard]] auto operator>=(const Bid& b1, const Bid& b2) -> bool {
  return !(b1 < b2);
}
[[nodiscard]] auto operator!=(const Bid& b1, const Bid& b2) -> bool {
  return !(b1 == b2);
}
