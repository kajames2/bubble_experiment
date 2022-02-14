#ifndef CLOCK_HH
#define CLOCK_HH

using Time = unsigned int;

namespace assetmarket {
class Clock {
 public:
  virtual Time GetTime() const = 0;
  virtual ~Clock() {}
};
}  // namespace assetmarket

#endif  // CLOCK_HH
