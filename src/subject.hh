#ifndef SUBJECT_HH
#define SUBJECT_HH

#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

namespace exptemplate {

using SubjectID = std::size_t;
struct Subject {
  SubjectID id;
  std::string name;
  int student_id;
  int payoff_cents;
};

auto PrettyPrint(const Subject& s) -> std::string;
std::ostream& operator<<(std::ostream&, const Subject&);
std::istream& operator>>(std::istream&, Subject&);

}  // namespace exptemplate

#endif  // SUBJECT_HH
