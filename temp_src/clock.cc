#include "clock.hh"

#include <algorithm>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, Time time) {
  return os
         << std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}
std::istream& operator>>(std::istream& is, Time& time) {
  unsigned int t;
  is >> t;
  time = static_cast<std::chrono::milliseconds>(t);
  return is;
}

}  // namespace assetmarket
