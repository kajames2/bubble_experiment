#ifndef ASK_HH
#define ASK_HH

#include <tuple>

struct Ask {
  unsigned int id;
  unsigned int player_id;
  unsigned int price;
  unsigned int timestamp;

  auto tie() const {
    int neg_price = -static_cast<int>(price);
    return std::tie(neg_price, timestamp);
  }
};

[[nodiscard]] auto operator==(const Ask& a1, const Ask& a2) -> bool;
[[nodiscard]] auto operator<(const Ask& a1, const Ask& a2) -> bool;
[[nodiscard]] auto operator>(const Ask& a1, const Ask& a2) -> bool;
[[nodiscard]] auto operator<=(const Ask& a1, const Ask& a2) -> bool;
[[nodiscard]] auto operator>=(const Ask& a1, const Ask& a2) -> bool;
[[nodiscard]] auto operator!=(const Ask& a1, const Ask& a2) -> bool;

#endif  // ASK_HH
