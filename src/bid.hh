#ifndef BID_HH
#define BID_HH

#include <tuple>

struct Bid {
  unsigned int id;
  unsigned int player_id;
  unsigned int price;
  unsigned int timestamp;

  auto tie() const { return std::tie(price, timestamp); }
};
[[nodiscard]] auto operator==(const Bid& b1, const Bid& b2) -> bool;
[[nodiscard]] auto operator<(const Bid& b1, const Bid& b2) -> bool;
[[nodiscard]] auto operator>(const Bid& b1, const Bid& b2) -> bool;
[[nodiscard]] auto operator<=(const Bid& b1, const Bid& b2) -> bool;
[[nodiscard]] auto operator>=(const Bid& b1, const Bid& b2) -> bool;
[[nodiscard]] auto operator!=(const Bid& b1, const Bid& b2) -> bool;

#endif  // BID_HH
