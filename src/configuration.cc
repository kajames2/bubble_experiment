#include "configuration.hh"

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const SubjectConfiguration& config) {
  os << static_cast<bool>(config.instructions_filename) << ";";
  if (config.instructions_filename) {
    os << *config.instructions_filename << "\n";
  }
  return os << config.init_alloc << ";" << config.is_creator << ";"
            << config.eperdollar;
}

std::istream& operator>>(std::istream& is, SubjectConfiguration& config) {
  bool has_instructions;
  is >> has_instructions;
  if (has_instructions) {
    is >> *config.instructions_filename;
  }
  is >> config.init_alloc;
  is >> config.is_creator;
  is >> config.eperdollar;
  return is;
}
}  // namespace assetmarket
