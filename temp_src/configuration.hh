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

struct RoundConfiguration {
  bool can_create;
  unsigned int round;
  unsigned int dividend;
  unsigned int margin_req;
  unsigned int margin_max;
};

using DividendSet = std::vector<unsigned int>;

struct Configuration {
  // Experiment Parameters
  unsigned int n_players = 0;
  unsigned int n_rounds = 10;
  bool valid_instructions;
  // Timing Parameters
  std::chrono::seconds round_time = std::chrono::seconds(60);
  std::chrono::seconds review_time = std::chrono::seconds(10);

  // Subject parameters
  std::unordered_map<SubjectID, SubjectConfiguration> sub_configs;
  std::vector<RoundConfiguration> per_configs;

  // Asset Market Parameters
  unsigned int first_create_round;
  DividendSet dividends;

  // Market Parameters
  bool replace_offer_rule;
  bool improvement_rule;

  auto GetSubjectConfiguration(SubjectID id) const -> SubjectConfiguration {
    return sub_configs.at(id);
  }

  auto GetRoundConfiguration(size_t round) const -> RoundConfiguration {
    return per_configs[round - 1];
  }
};

std::string PrettyPrint(const SubjectConfiguration& config);
std::ostream& operator<<(std::ostream& os, const SubjectConfiguration& config);
std::istream& operator>>(std::istream& is, SubjectConfiguration& config);

std::string PrettyPrint(const RoundConfiguration& config);
std::ostream& operator<<(std::ostream& os, const RoundConfiguration& config);
std::istream& operator>>(std::istream& is, RoundConfiguration& config);

}  // namespace assetmarket

#endif  // CONFIGURATION_H
