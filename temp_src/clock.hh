#ifndef CLOCK_HH
#define CLOCK_HH

#include <chrono>
#include <istream>
#include <ostream>

using Time = std::chrono::duration<float>;

namespace assetmarket {
class Clock {
 public:
  virtual Time GetTimeElapsed() const = 0;
  virtual Time GetTimeRemaining() const = 0;
  virtual ~Clock() {}
};

std::ostream& operator<<(std::ostream& os, Time time);
std::istream& operator>>(std::istream& is, Time& time);
}  // namespace assetmarket

#endif  // CLOCK_HH
