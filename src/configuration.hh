#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "portfolio.hh"
#include "subject.hh"

namespace assetmarket {

struct SubjectConfiguration {
  bool is_creator;
  std::optional<std::string> instructions_filename;
  double eperdollar;
  Portfolio init_alloc;
};

using DividendSet = std::vector<int>;

struct Configuration {
  // Experiment Parameters
  unsigned int n_players;
  unsigned int n_rounds = 10;
  bool valid_instructions;
  // Timing Parameters
  std::chrono::seconds round_time = std::chrono::seconds(60);
  std::chrono::seconds review_time = std::chrono::seconds(10);

  // Subject parameters
  std::unordered_map<SubjectID, SubjectConfiguration> sub_configs;

  // Asset Market Parameters
  DividendSet dividends;
  float margin_requirement;
  unsigned int first_create_round;

  // Market Parameters
  bool replace_offer_rule;
  bool improvement_rule;

  auto GetSubjectConfiguration(SubjectID id) const -> SubjectConfiguration {
    return sub_configs.at(id);
  }
};

std::ostream& operator<<(std::ostream& os, const SubjectConfiguration& config);
std::istream& operator>>(std::istream& is, SubjectConfiguration& config);

}  // namespace assetmarket

#endif  // CONFIGURATION_H
