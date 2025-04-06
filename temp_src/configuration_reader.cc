#include "configuration_reader.hh"

#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <filesystem>
#include <iostream>
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
    if (!std::filesystem::exists(filename)) {
      return ConfigError::FileDoesNotExist;
    }
    read_json(filename, root);

    if (!root.count("round_seconds") || !root.count("review_seconds") ||
        !root.count("n_players") || !root.count("n_rounds") ||
        !root.count("replace_offer_rule") || !root.count("improvement_rule") ||
        !root.count("role_assignments") || !root.count("roles") ||
        !root.count("dividends") || !root.count("dividend_realizations") ||
        !root.count("margin_requirements")) {
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
    auto dividend_realizations = ReadDividendRealizations(root);
    auto margin_requirements = ReadMarginRequirements(root);
    auto margin_max_requirements = ReadMarginMax(root);

    std::unordered_map<std::string, SubjectConfiguration> archetypes =
        ReadRoleArchetypes(root.get_child("roles"));

    std::vector<std::string> roles = ReadRoleAssignments(root);
    if (roles.size() < config.n_players) {
      return ConfigError::TooFewRoleAssignments;
    }
    if (roles.size() > config.n_players) {
      warnings.push_back(ConfigWarning::TooManyRoleAssignments);
    }
    if (dividend_realizations.size() < config.n_rounds) {
      return ConfigError::TooFewDividendRealizations;
    }
    if (margin_requirements.size() < config.n_rounds) {
      return ConfigError::TooFewMarginRequirements;
    }
    if (margin_max_requirements.size() < config.n_rounds) {
      return ConfigError::TooFewMarginRequirements;
    }
    if (dividend_realizations.size() > config.n_rounds) {
      warnings.push_back(ConfigWarning::TooManyDividendRealizations);
    }
    if (margin_requirements.size() > config.n_rounds) {
      warnings.push_back(ConfigWarning::TooManyMarginRequirements);
    }
    if (margin_max_requirements.size() > config.n_rounds) {
      warnings.push_back(ConfigWarning::TooManyMarginRequirements);
    }
    for (auto i : dividend_realizations) {
      if (i >= config.dividends.size()) {
        return ConfigError::InvalidDividendRealization;
      }
    }
    for (unsigned int i = 0; i < config.n_rounds; ++i) {
      auto round = i + 1;
      config.per_configs.push_back({round >= config.first_create_round, round,
                                    config.dividends[dividend_realizations[i]],
                                    margin_requirements[i],
                                    margin_max_requirements[i]});
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
  std::vector<unsigned int> dividends;
  for (auto& cell : root.get_child("dividends")) {
    dividends.emplace_back(cell.second.get_value<unsigned int>());
  }
  return DividendSet(dividends);
}

std::vector<unsigned int> ReadDividendRealizations(
    boost::property_tree::ptree root) {
  std::vector<unsigned int> reals;
  for (auto& cell : root.get_child("dividend_realizations")) {
    reals.emplace_back(cell.second.get_value<unsigned int>());
  }
  return reals;
}

std::vector<unsigned int> ReadMarginRequirements(
    boost::property_tree::ptree root) {
  std::vector<unsigned int> margs;
  for (auto& cell : root.get_child("margin_requirements")) {
    margs.emplace_back(cell.second.get_value<unsigned int>());
  }
  return margs;
}

std::vector<unsigned int> ReadMarginMax(boost::property_tree::ptree root) {
  std::vector<unsigned int> margs;
  for (auto& cell : root.get_child("max_margins")) {
    margs.emplace_back(cell.second.get_value<unsigned int>());
  }
  return margs;
}

std::string PrettyPrint(ConfigError err) {
  std::string s = "File failed to load!  ERROR: ";
  switch (err) {
    case ConfigError::FileDoesNotExist:
      return s + "File does not exist";
    case ConfigError::InvalidFormat:
      return s + "Invalid format";
    case ConfigError::MissingValue:
      return s + "Required parameter missing";
    case ConfigError::TooFewRoleAssignments:
      return s + "Too few role assignments";
    case ConfigError::UnknownRole:
      return s + "Unknown role";
    case ConfigError::InvalidDividendRealization:
      return s + "Invalid Dividend Realization";
    case ConfigError::TooFewDividendRealizations:
      return s + "Too few Dividend realizations";
    case ConfigError::TooFewMarginRequirements:
      return s + "Too few Margin Requirements";
    default:
      return s + "Unknown Error... uh oh";
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
    case ConfigWarning::TooManyDividendRealizations:
      return s + "More dividend realizations than rounds";
    case ConfigWarning::TooManyMarginRequirements:
      return s + "More Margin Requirements than rounds";
    default:
      return s + "Unknown Warning... uh oh";
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

std::istream& operator>>(std::istream& is, ConfigError& err) {
  int cerr;
  is >> cerr;
  err = static_cast<ConfigError>(cerr);
}

std::istream& operator>>(std::istream& is, ConfigWarning& warn) {
  int cwarn;
  is >> cwarn;
  warn = static_cast<ConfigWarning>(cwarn);
  return is;
}

std::ostream& operator<<(std::ostream& os, ConfigResult res) {
  bool has_error = static_cast<bool>(res.error);
  os << has_error;
  if (has_error) {
    os << " " << *res.error;
  }
  os << " " << res.warnings.size();
  for (auto warn : res.warnings) {
    os << " " << warn;
  }
  return os;
}

std::istream& operator>>(std::istream& is, ConfigResult& res) {
  bool has_error;
  is >> has_error;
  res.error = std::nullopt;
  if (has_error) {
    ConfigError err;
    is >> err;
    res.error = err;
  }
  int n_warn;
  is >> n_warn;
  for (int i = 0; i < n_warn; ++i) {
    ConfigWarning warn;
    is >> warn;
    res.warnings.push_back(warn);
  }
  return is;
}

}  // namespace assetmarket
