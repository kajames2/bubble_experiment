#include "configuration.hh"

#include <sstream>
#include <string>

namespace assetmarket {

std::string PrettyPrint(const SubjectConfiguration& config) {
  std::string out = "Configuration:\n";
  out += "\tInstructions File: ";
  if (config.instructions_filename) {
    out += *config.instructions_filename;
  } else {
    out += "N/A";
  }
  out += '\n';
  out += "Is Creator? " + std::to_string(config.is_creator) + '\n';
  out +=
      "Exchange_rate: " + std::to_string(config.eperdollar) + "E per $1 USD\n";
  out += "Initial ";
  out += PrettyPrint(config.init_alloc);
  return out;
}

std::ostream& operator<<(std::ostream& os, const SubjectConfiguration& config) {
  os << static_cast<bool>(config.instructions_filename) << " ";
  if (config.instructions_filename) {
    os << *config.instructions_filename << "\n";
  }
  return os << config.init_alloc << " " << config.is_creator << " "
            << config.eperdollar;
}

std::istream& operator>>(std::istream& is, SubjectConfiguration& config) {
  bool has_instructions;
  is >> has_instructions;
  if (has_instructions) {
    std::string filename;
    getline(is, filename);
    config.instructions_filename = filename;
  }
  is >> config.init_alloc;
  is >> config.is_creator;
  is >> config.eperdollar;
  return is;
}

std::string PrettyPrint(const RoundConfiguration& config) {
  std::stringstream ss;
  ss << "Round Configuration:\n";
  ss << "\tRound: " << config.round << "\n";
  ss << "\tCreation Round: " << config.can_create << "\n";
  ss << "\tDividend Payout: " << config.dividend << "\n";
  ss << "\tMargin Per Share: " << config.margin_req;
  ss << "\tMax Margin Per Share: " << config.margin_max;
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const RoundConfiguration& config) {
  return os << config.round << " " << config.can_create << " "
            << config.dividend << " " << config.margin_req << " "
            << config.margin_max;
}

std::istream& operator>>(std::istream& is, RoundConfiguration& config) {
  return is >> config.round >> config.can_create >> config.dividend >>
         config.margin_req >> config.margin_max;
}
}  // namespace assetmarket
