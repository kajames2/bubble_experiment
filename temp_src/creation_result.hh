#ifndef CREATION_RESULT_HH
#define CREATION_RESULT_HH

#include <istream>
#include <ostream>

#include "subject.hh"
#include <string>

namespace assetmarket {

enum class CreationValidity {
  Accepted,
  RejectedMarketClosed,
  RejectedCannotCreate,
  RejectedInsufficientCash
};

struct CreationResult {
  CreationValidity validity;
  SubjectID id;
};

std::string PrettyPrint(CreationValidity val);
std::ostream& operator<<(std::ostream& os, const CreationResult& rr);
std::istream& operator>>(std::istream& is, CreationResult& rr);
}  // namespace assetmarket

#endif  // CREATION_RESULT_HH
