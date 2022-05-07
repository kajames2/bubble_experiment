#include "configuration_reader.hh"

#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "configuration.hh"

using namespace boost::property_tree;

namespace assetmarket {

std::variant<ConfigError, std::pair<Configuration, std::vector<ConfigWarning>>>
ReadConfigurationFile(std::string filename) {
  try {
    ptree root;
    std::vector<ConfigWarning> warnings;
    read_json(filename, root);

    if (!root.count("round_seconds") || !root.count("review_seconds") ||
        !root.count("n_players") || !root.count("n_rounds") ||
        !root.count("replace_offer_rule") || !root.count("improvement_rule") ||
        !root.count("role_assignments") || !root.count("roles")) {
      return ConfigError::MissingValue;
    }

    Configuration config;
    config.round_time = std::chrono::seconds(root.get<int>("round_seconds"));
    config.review_time = std::chrono::seconds(root.get<int>("review_seconds"));
    config.n_players = root.get<unsigned int>("n_players");
    config.n_rounds = root.get<unsigned int>("n_rounds");
    config.replace_offer_rule = root.get<bool>("replace_offer_rule");
    config.improvement_rule = root.get<bool>("improvement_rule");
    config.first_create_round = root.get<unsigned int>("first_create_round");
    config.dividends = ReadDividends(root);
    config.margin_requirement = root.get<float>("sequestration_fraction");

    std::unordered_map<std::string, SubjectConfiguration> archetypes =
        ReadRoleArchetypes(root.get_child("roles"));

    std::vector<std::string> roles = ReadRoleAssignments(root);
    if (roles.size() < config.n_players) {
      return ConfigError::TooFewRoleAssignments;
    }
    if (roles.size() > config.n_players) {
      warnings.push_back(ConfigWarning::TooManyRoleAssignments);
    }

    bool valid_instructions = true;
    std::map<int, std::string> instructions;
    for (size_t i = 0; i < config.n_players; ++i) {
      auto role = roles[i];
      if (!archetypes.contains(role)) {
        return ConfigError::UnknownRole;
      }
      auto settings = archetypes[role];

      if (valid_instructions) {
        if (settings.instructions_filename == "") {
          valid_instructions = false;
          warnings.push_back(ConfigWarning::MustIgnoreInstructions);
        }
      }
      config.sub_configs[i] = settings;
    }

    config.valid_instructions = valid_instructions;
    return std::pair<Configuration, std::vector<ConfigWarning>>(config,
                                                                warnings);
  } catch (std::exception& e) {
    return ConfigError::InvalidFormat;
  }
}

std::unordered_map<std::string, SubjectConfiguration> ReadRoleArchetypes(
    boost::property_tree::ptree role_config) {
  std::unordered_map<std::string, SubjectConfiguration> settings;
  for (auto& pconfig : role_config) {
    SubjectConfiguration ps;
    ps.eperdollar = pconfig.second.get<double>("e_per_dollar");
    ps.is_creator = pconfig.second.get<bool>("is_creator");
    ps.instructions_filename =
        pconfig.second.get<std::string>("instructions_filename", "");
    ps.init_alloc.Set(Item::Cash, pconfig.second.get<int>("init_cash"));
    ps.init_alloc.Set(Item::Shares, pconfig.second.get<int>("init_units"));
    settings.emplace(pconfig.second.get<std::string>("name"), ps);
  }
  return settings;
}

std::vector<std::string> ReadRoleAssignments(boost::property_tree::ptree root) {
  std::vector<std::string> role_assignments;
  for (auto& cell : root.get_child("role_assignments")) {
    role_assignments.emplace_back(cell.second.get_value<std::string>());
  }
  return role_assignments;
}

DividendSet ReadDividends(boost::property_tree::ptree root) {
  std::vector<int> dividends;
  for (auto& cell : root.get_child("dividends")) {
    dividends.emplace_back(cell.second.get_value<int>());
  }
  return DividendSet(dividends);
}

std::string PrettyPrint(ConfigError err) {
  std::string s = "File failed to load!  ERROR: ";
  switch (err) {
    case ConfigError::InvalidFormat:
      return s + "Invalid format";
    case ConfigError::MissingValue:
      return s + "Required parameter missing";
    case ConfigError::TooFewRoleAssignments:
      return s + "Too few role assignments";
    case ConfigError::UnknownRole:
      return s + "Unknown role";
    case ConfigError::TooFewDividendPayments:
      return s + "Too few Dividend payments";
  }
}

std::string PrettyPrint(ConfigWarning warn) {
  std::string s = "WARNING: ";
  switch (warn) {
    case ConfigWarning::UnusedRoles:
      return s + "Some roles are unused";
    case ConfigWarning::TooManyRoleAssignments:
      return s + "More role assignments than players";
    case ConfigWarning::MustIgnoreInstructions:
      return s +
             "Instructions will be skipped: cannot read instruction files for "
             "all players";
    case ConfigWarning::TooManyDividendPayments:
      return s + "More role dividend payments than rounds";
  }
}

std::string PrettyPrint(ConfigResult res) {
  std::string out;
  if (!res.error) {
    out += "Configuration Loaded.\n";
  } else {
    out += PrettyPrint(*res.error) + "\n";
  }
  for (auto warn : res.warnings) {
    out += PrettyPrint(warn) + "\n";
  }
  return out;
}

std::ostream& operator<<(std::ostream& os, ConfigError err) {
  return os << static_cast<int>(err);
}

std::ostream& operator<<(std::ostream& os, ConfigWarning warn) {
  return os << static_cast<int>(warn);
}

std::ostream& operator<<(std::ostream& os, ConfigResult res) {
  bool has_error = static_cast<bool>(res.error);
  os << has_error;
  if (has_error) {
    os << ";" << *res.error;
  }
  os << ";" << res.warnings.size();
  for (auto warn : res.warnings) {
    os << ";" << warn;
  }
  return os;
}

std::istream& operator>>(std::istream& is, ConfigResult& res) {
  bool has_error;
  is >> has_error;
  res.error = std::nullopt;
  if (has_error) {
    int err;
    is >> err;
    *res.error = static_cast<ConfigError>(err);
  }
  int n_warn;
  is >> n_warn;
  for (int i = 0; i < n_warn; ++i) {
    int warn;
    is >> warn;
    res.warnings.push_back(static_cast<ConfigWarning>(warn));
  }
  return is;
}

}  // namespace assetmarket
