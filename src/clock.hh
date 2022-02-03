#ifndef CLOCK_HH
#define CLOCK_HH

using Time = unsigned int;

class Clock {
 public:
  virtual Time GetTime() const = 0;
  virtual ~Clock() {}
};

#endif  // CLOCK_HH
