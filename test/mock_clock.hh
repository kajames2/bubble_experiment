#ifndef MOCK_CLOCK_HH
#define MOCK_CLOCK_HH

#include "clock.hh"

namespace assettest {
class MockClock : public assetmarket::Clock {
 public:
  void SetTime(unsigned int time) { time_ = time; }
  Time GetTimeElapsed() const override { return time_; }

 private:
  unsigned int time_;
};
}  // namespace assettest
#endif  // MOCK_CLOCK_HH
