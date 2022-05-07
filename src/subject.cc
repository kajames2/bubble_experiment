#include "subject.hh"

namespace assetmarket {
std::ostream& operator<<(std::ostream& os, const Subject& s) {
  return os << s.id << ";" << s.name << "\n"
            << s.student_id << ";" << s.payoff_cents;
}
std::istream& operator>>(std::istream& is, Subject& s) {
  is >> s.id;
  is >> s.name;
  is >> s.student_id;
  is >> s.payoff_cents;
  return is;
}
}  // namespace assetmarket
