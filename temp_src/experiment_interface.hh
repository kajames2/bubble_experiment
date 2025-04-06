#ifndef EXPERIMENT_INTERFACE_HH
#define EXPERIMENT_INTERFACE_HH

#include <string>
#include <vector>

#include "asset_market_enums.hh"
#include "configuration.hh"
#include "subject.hh"

namespace assetmarket {
class ExperimentInterface {
 public:
  virtual auto StartExperiment() -> void = 0;
  virtual auto EndExperiment() -> void = 0;
  virtual auto StartInstructions() -> void = 0;
  virtual auto EndInstructions() -> void = 0;
  virtual auto StartRound() -> void = 0;
  virtual auto EndRound() -> void = 0;
  virtual auto StartDividends() -> void = 0;
  virtual auto EndDividends() -> void = 0;
  virtual auto StartReview() -> void = 0;
  virtual auto EndReview() -> void = 0;
  virtual auto Pause() -> void = 0;
  virtual auto Resume() -> void = 0;
  virtual auto StartPayouts() -> void = 0;
  virtual auto GetRound() const -> unsigned int = 0;

  virtual auto GetSubject(SubjectID id) const -> Subject = 0;
  virtual auto GetSubjects() const -> std::vector<Subject> = 0;
  virtual auto GetSubjectIDs() const -> std::vector<SubjectID> = 0;
  virtual auto AddSubject() -> SubjectID = 0;
  virtual auto TakeName(SubjectID id, std::string name) -> void = 0;
  virtual auto SetPayoff(SubjectID id, int payoff_cents) -> void = 0;
  virtual auto SetConfiguration(const Configuration& config) -> void = 0;
  virtual auto GetStatus() const -> Status = 0;
  virtual auto GetStage() const -> Stage = 0;

  virtual ~ExperimentInterface() {}
};
}  // namespace assetmarket
#endif  // EXPERIMENT_INTERFACE_HH
