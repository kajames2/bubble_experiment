#ifndef EXPERIMENT_CONTROLLER_HH
#define EXPERIMENT_CONTROLLER_HH

#include <memory>

#include "connection_info.hh"
#include "experiment_interface.hh"
#include "server.hh"

namespace assetmarket {

class ExperimentController {
 public:
  ExperimentController(std::shared_ptr<Server> serv,
                       std::shared_ptr<ExperimentInterface> exp)
      : serv_(serv), exp_(exp) {}
  auto StartExperiment() -> void {
    exp_->StartExperiment();
    serv_->SendAll(
        assetmarket::Message(assetmarket::MessageType::ExperimentStarted));
  }

 private:
  std::shared_ptr<Server> serv_;
  std::shared_ptr<ExperimentInterface> exp_;
};

}  // namespace assetmarket

#endif  // EXPERIMENT_CONTROLLER_HH
