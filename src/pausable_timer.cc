#include "pausable_timer.hh"

#include <string>

namespace assetmarket {

PausableTimer::PausableTimer(asio::io_context& io,
                             std::chrono::seconds duration)
    : state_(TimerState::NotStarted),
      timer_(io, duration),
      duration_(duration),
      last_start_(std::chrono::system_clock::now()),
      accum_time_(0) {}

auto PausableTimer::Clear() -> void {
  callbacks_.clear();
  Reset();
}

auto PausableTimer::Reset() -> void {
  accum_time_ = Time(0);
  state_ = TimerState::NotStarted;
  timer_.cancel();
}

auto PausableTimer::Restart() -> void {
  Reset();
  Start();
}

auto PausableTimer::Pause() -> void {
  accum_time_ = GetTimeElapsed();
  state_ = TimerState::Paused;
  timer_.cancel();
}

auto PausableTimer::Start() -> void {
  if (state_ == TimerState::Running || state_ == TimerState::Ended) {
    return;
  }
  timer_.expires_after(std::chrono::duration_cast<std::chrono::milliseconds>(
      GetTimeRemaining()));
  last_start_ = std::chrono::system_clock::now();
  timer_.async_wait([this](const std::error_code& e) { OnStop(e); });
  state_ = TimerState::Running;
}

auto PausableTimer::GetTimeRemaining() const -> Time {
  return duration_ - GetTimeElapsed();
}

auto PausableTimer::GetTimeElapsed() const -> Time {
  Time elapsed = accum_time_;
  if (state_ == TimerState::Running) {
    elapsed += (std::chrono::system_clock::now() - last_start_);
  }
  return elapsed;
}

auto PausableTimer::OnStop(const std::error_code& e) -> void {
  if (e == asio::error::operation_aborted) {
    return;
  }
  state_ = TimerState::Ended;
  accum_time_ = duration_;
  for (auto& callback : callbacks_) {
    callback();
  }
}

auto ToString(TimerState s) -> std::string {
  switch (s) {
    case TimerState::NotStarted:
      return "NotStarted";
    case TimerState::Running:
      return "Running";
    case TimerState::Paused:
      return "Paused";
    case TimerState::Ended:
      return "Ended";
  }
}

std::istream& operator>>(std::istream& is, TimerState& s) {
  static std::string NotStartedString = ToString(TimerState::NotStarted);
  static std::string RunningString = ToString(TimerState::Running);
  static std::string PausedString = ToString(TimerState::Paused);
  static std::string EndedString = ToString(TimerState::Ended);

  std::string str;
  is >> str;
  if (str == NotStartedString) {
    s = TimerState::NotStarted;
  } else if (str == RunningString) {
    s = TimerState::Running;
  } else if (str == PausedString) {
    s = TimerState::Paused;
  } else if (str == EndedString) {
    s = TimerState::Ended;
  }
  return is;
}

std::ostream& operator<<(std::ostream& os, const TimerState& s) {
  return os << ToString(s);
}

}  // namespace assetmarket
