#ifndef CLOCK_HH
#define CLOCK_HH

class Clock {
 public:
  virtual unsigned int Time() const = 0;
  virtual ~Clock() {}
};

#endif  // CLOCK_HH
