#ifndef TOOLS_PAUSABLE_TIMER_H
#define TOOLS_PAUSABLE_TIMER_H

#include <asio.hpp>
#include <chrono>
#include <functional>
#include <vector>

#include "clock.hh"

namespace assetmarket {

enum class TimerState { NotStarted, Running, Paused, Ended };
std::ostream& operator<<(std::ostream& os, const TimerState& s);
std::istream& operator>>(std::istream& is, TimerState& s);

class PausableTimer : public Clock {
 public:
  PausableTimer(asio::io_context& io, std::chrono::milliseconds duration =
                                          std::chrono::milliseconds(0));
  virtual Time GetTimeRemaining() const override;
  virtual Time GetTimeElapsed() const override;
  void Reset();
  void Pause();
  void Start();
  void Restart();
  void Clear();
  void Register(std::function<void()> callback) {
    callbacks_.push_back(callback);
  }
  void SetDuration(std::chrono::milliseconds duration) {
    duration_ = duration;
    if (state_ == TimerState::Running) {
      Pause();
      Start();
    }
  }
  void UpdateTime(std::chrono::milliseconds duration) {
    bool start_again = false;
    if (state_ == TimerState::Running) {
      Pause();
      start_again = true;
      accum_time_ = duration_ - duration;
      Start();
    } else {
      duration_ = duration;
    }
  }

  TimerState GetState() const { return state_; }

 private:
  void OnStop(const std::error_code& e);

  TimerState state_;
  asio::steady_timer timer_;
  Time duration_;
  std::chrono::time_point<std::chrono::system_clock> last_start_;
  Time accum_time_;
  std::vector<std::function<void()>> callbacks_;
};

}  // namespace assetmarket

#endif  // TOOLS_PAUSABLE_TIMER_H
