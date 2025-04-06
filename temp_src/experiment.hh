#ifndef EXPERIMENT_HH
#define EXPERIMENT_HH

#include <memory>
#include <string>
#include <vector>

#include "asset_market_enums.hh"
#include "configuration.hh"
#include "experiment_interface.hh"
#include "experiment_state.hh"
#include "subject.hh"

namespace assetmarket {

class Experiment : public ExperimentInterface {
 public:
  Experiment(std::shared_ptr<Configuration> config,
             std::shared_ptr<ExperimentState> state)
      : config_(config), exp_state_(state) {}
  auto NSubjects() const -> size_t { return subjects_.size(); }
  auto AddSubject(Subject p) -> SubjectID {
    SubjectID id = subjects_.size();
    subjects_[id] = p;
    return id;
  }
  auto AddSubject() -> SubjectID override {
    SubjectID id = subjects_.size();
    subjects_[id] = {id, "", 0, 0};
    return id;
  }
  auto GetStatus() const -> Status override { return exp_state_->status; }
  auto GetStage() const -> Stage override { return exp_state_->stage; }
  auto GetRound() const -> unsigned int override { return exp_state_->round; }
  auto GetSubject(SubjectID id) const -> Subject override {
    return subjects_.at(id);
  }
  auto GetSubjectIDs() const -> std::vector<SubjectID> override {
    std::vector<SubjectID> ids;
    for (auto [key, value] : subjects_) {
      ids.push_back(value.id);
    }
    return ids;
  }
  auto GetSubjects() const -> std::vector<Subject> override {
    std::vector<Subject> subs;
    for (auto [key, value] : subjects_) {
      subs.push_back(value);
    }
    return subs;
  };

  auto StartExperiment() -> void override {
    exp_state_->status = Status::Running;
  }
  auto EndExperiment() -> void override { exp_state_->status = Status::Ended; }

  auto StartInstructions() -> void override {
    exp_state_->stage = Stage::Instructions;
  }
  auto EndInstructions() -> void override {}

  auto StartRound() -> void override {
    if (GetStage() == Stage::PreExperiment) {
      exp_state_->round = 1;
    } else {
      exp_state_->round += 1;
    }
    exp_state_->status = Status::Running;
    exp_state_->stage = Stage::Round;
  }
  auto EndRound() -> void override {}
  auto StartDividends() -> void override {
    exp_state_->stage = Stage::Dividends;
  }
  auto EndDividends() -> void override {}
  auto StartReview() -> void override { exp_state_->stage = Stage::Review; }
  auto EndReview() -> void override {}

  auto Pause() -> void override { exp_state_->status = Status::Paused; }
  auto Resume() -> void override { exp_state_->status = Status::Running; }
  auto StartPayouts() -> void override { exp_state_->stage = Stage::Payouts; }
  auto TakeName(SubjectID id, std::string name) -> void override {
    subjects_[id].name = name;
  }
  auto SetPayoff(SubjectID id, int payoff_cents) -> void override {
    subjects_[id].payoff_cents = payoff_cents;
  }

  auto SetConfiguration(const Configuration& config) -> void override {
    *config_ = config;
  }

 private:
  std::shared_ptr<Configuration> config_;
  std::unordered_map<SubjectID, Subject> subjects_;
  std::shared_ptr<ExperimentState> exp_state_;
};
}  // namespace assetmarket

#endif  // EXPERIMENT_HH
