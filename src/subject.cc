#include "subject.hh"

#include <sstream>
#include <string>

namespace exptemplate {
auto PrettyPrint(const Subject& s) -> std::string {
  std::stringstream ss;
  ss << s.id << " " << s.student_id << " " << s.name << " " << s.payoff_cents;
  return ss.str();
}
std::ostream& operator<<(std::ostream& os, const Subject& s) {
  return os << s.id << " " << s.name << "\n"
            << s.student_id << " " << s.payoff_cents;
}
std::istream& operator>>(std::istream& is, Subject& s) {
  is >> s.id;
  getline(is, s.name);
  is >> s.student_id;
  is >> s.payoff_cents;
  return is;
}
}  // namespace exptemplate
