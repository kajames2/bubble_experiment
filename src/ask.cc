#include "ask.hh"

[[nodiscard]] auto operator==(const Ask& a1, const Ask& a2) -> bool {
  return a1.tie() == a2.tie();
}
[[nodiscard]] auto operator<(const Ask& a1, const Ask& a2) -> bool {
  return a1.tie() < a2.tie();
}
[[nodiscard]] auto operator>(const Ask& a1, const Ask& a2) -> bool {
  return !((a1 < a2) || (a1 == a2));
}
[[nodiscard]] auto operator<=(const Ask& a1, const Ask& a2) -> bool {
  return (a1 < a2) || (a1 == a2);
}
[[nodiscard]] auto operator>=(const Ask& a1, const Ask& a2) -> bool {
  return !(a1 < a2);
}
[[nodiscard]] auto operator!=(const Ask& a1, const Ask& a2) -> bool {
  return !(a1 == a2);
}
