#ifndef SUBJECT_HH
#define SUBJECT_HH

#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

namespace assetmarket {

using SubjectID = std::size_t;
struct Subject {
  SubjectID id;
  std::string name;
  int student_id;
  int payoff_cents;
};

std::ostream& operator<<(std::ostream&, const Subject&);
std::istream& operator>>(std::istream&, Subject&);

}  // namespace assetmarket

#endif  // SUBJECT_HH
