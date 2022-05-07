#ifndef CLOCK_HH
#define CLOCK_HH

#include <chrono>

using Time = std::chrono::duration<float>;

namespace assetmarket {
class Clock {
 public:
  virtual Time GetTimeElapsed() const = 0;
  virtual Time GetTimeRemaining() const = 0;
  virtual ~Clock() {}
};
}  // namespace assetmarket

#endif  // CLOCK_HH
