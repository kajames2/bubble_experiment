#ifndef MOCK_EXPERIMENT
#define MOCK_EXPERIMENT

#include <vector>

#include "experiment_interface.hh"

namespace assettest {

class MockExperiment : public assetmarket::ExperimentInterface {
 public:
  auto StartExperiment() -> void override { n_start_exp += 1; }
  auto AddSubject() -> assetmarket::SubjectID override {
    used_ids.push_back(n_subjects);
    return n_subjects++;
  }

  int n_start_exp = 0;
  unsigned int n_subjects = 0;
  std::vector<assetmarket::SubjectID> used_ids;
};
}  // namespace assettest

#endif  // MOCK_EXPERIMENT
