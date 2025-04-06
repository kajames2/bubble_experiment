#ifndef CONFIGURATION_READER_HH
#define CONFIGURATION_READER_HH

#include <boost/property_tree/ptree.hpp>
#include <ostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "configuration.hh"

namespace assetmarket {

enum class ConfigError {
  FileDoesNotExist,
  InvalidFormat,
  MissingValue,
  TooFewRoleAssignments,
  UnknownRole,
  InvalidDividendRealization,
  TooFewDividendRealizations,
  TooFewMarginRequirements
};

enum class ConfigWarning {
  UnusedRoles,
  TooManyRoleAssignments,
  MustIgnoreInstructions,
  TooManyDividendRealizations,
  TooManyMarginRequirements
};

struct ConfigResult {
  std::optional<ConfigError> error = std::nullopt;
  std::vector<ConfigWarning> warnings = {};
};

std::string PrettyPrint(ConfigError);
std::string PrettyPrint(ConfigWarning);
std::string PrettyPrint(ConfigResult);
std::ostream& operator<<(std::ostream&, ConfigError);
std::ostream& operator<<(std::ostream&, ConfigWarning);
std::istream& operator>>(std::istream&, ConfigError&);
std::istream& operator>>(std::istream&, ConfigWarning&);
std::ostream& operator<<(std::ostream&, ConfigResult);
std::istream& operator>>(std::istream&, ConfigResult&);

auto ReadConfigurationFile(std::string filename)
    -> std::variant<ConfigError,
                    std::pair<Configuration, std::vector<ConfigWarning>>>;
auto ReadRoleAssignments(boost::property_tree::ptree root)
    -> std::vector<std::string>;
auto ReadRoleArchetypes(boost::property_tree::ptree root)
    -> std::unordered_map<std::string, SubjectConfiguration>;
auto ReadDividends(boost::property_tree::ptree root) -> DividendSet;
auto ReadDividendRealizations(boost::property_tree::ptree root)
    -> std::vector<unsigned int>;
auto ReadMarginRequirements(boost::property_tree::ptree root)
    -> std::vector<unsigned int>;
auto ReadMarginMax(boost::property_tree::ptree root)
    -> std::vector<unsigned int>;
}  // namespace assetmarket

#endif  // CONFIGURATION_READER_HH
