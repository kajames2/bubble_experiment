#ifndef EXPERIMENT_STATE_HH
#define EXPERIMENT_STATE_HH

#include "asset_market_enums.hh"

namespace assetmarket {

struct ExperimentState {
  unsigned int round = 0;
  Status status = Status::NotStarted;
  Stage stage = Stage::PreExperiment;

  auto IsMarketOpen() const -> bool {
    return status == Status::Running && stage == Stage::Round;
  }
};

}  // namespace assetmarket

#endif  // EXPERIMENT_STATE_HH
