#ifndef EXPERIMENT_HH
#define EXPERIMENT_HH

#include <vector>

#include "configuration.hh"
#include "experiment_interface.hh"
#include "subject.hh"

namespace assetmarket {

enum class Stage { NotStarted, Started };

class Experiment : public ExperimentInterface {
 public:
  auto NSubjects() const -> size_t { return subjects_.size(); }
  auto AddSubject(Subject p) -> SubjectID {
    subjects_.push_back(p);
    return subjects_.size();
  }
  auto AddSubject() -> SubjectID override {}
  auto StartExperiment() -> void override {}
  auto GetStage() { return stage_; }
  auto NewPeriod() {
    if (GetStage() != Stage::NotStarted) {
      period_ += 1;
    }
  }
  auto GetPeriod() const { return period_; }
  auto Start() { stage_ = Stage::Started; }

 private:
  std::vector<Subject> subjects_;
  int period_ = 0;
  Stage stage_ = Stage::NotStarted;
};
}  // namespace assetmarket

#endif  // EXPERIMENT_HH
