#include "offer.hh"

[[nodiscard]] auto operator==(const Offer& off1, const Offer& off2) -> bool {
  return off1.tie() == off2.tie();
}
[[nodiscard]] auto operator<(const Offer& off1, const Offer& off2) -> bool {
  return off1.tie() < off2.tie();
}
[[nodiscard]] auto operator>(const Offer& off1, const Offer& off2) -> bool {
  return !((off1 < off2) || (off1 == off2));
}
[[nodiscard]] auto operator<=(const Offer& off1, const Offer& off2) -> bool {
  return (off1 < off2) || (off1 == off2);
}
[[nodiscard]] auto operator>=(const Offer& off1, const Offer& off2) -> bool {
  return !(off1 < off2);
}
[[nodiscard]] auto operator!=(const Offer& off1, const Offer& off2) -> bool {
  return !(off1 == off2);
}
