#ifndef EXPERIMENT_INTERFACE_HH
#define EXPERIMENT_INTERFACE_HH

#include "subject.hh"

namespace assetmarket {
class ExperimentInterface {
 public:
  virtual auto StartExperiment() -> void = 0;
  virtual auto AddSubject() -> SubjectID = 0;
  virtual ~ExperimentInterface() {}
};
}  // namespace assetmarket
#endif  // EXPERIMENT_INTERFACE_HH
