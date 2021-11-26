#ifndef OFFER_HH
#define OFFER_HH

#include <tuple>

struct Offer {
  unsigned int id;
  unsigned int player_id;
  unsigned int price;
  unsigned int timestamp;

  auto tie() const { return std::tie(price, timestamp); }
};

[[nodiscard]] auto operator==(const Offer& off1, const Offer& off2) -> bool;
[[nodiscard]] auto operator<(const Offer& off1, const Offer& off2) -> bool;
[[nodiscard]] auto operator>(const Offer& off1, const Offer& off2) -> bool;
[[nodiscard]] auto operator<=(const Offer& off1, const Offer& off2) -> bool;
[[nodiscard]] auto operator>=(const Offer& off1, const Offer& off2) -> bool;
[[nodiscard]] auto operator!=(const Offer& off1, const Offer& off2) -> bool;

#endif  // OFFER_HH
