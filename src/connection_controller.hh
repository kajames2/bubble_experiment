#ifndef CONNECTION_CONTROLLER_HH
#define CONNECTION_CONTROLLER_HH

#include <memory>

#include "connection_info.hh"
#include "experiment_interface.hh"
#include "server.hh"

namespace assetmarket {

class ConnectionController {
 public:
  ConnectionController(std::shared_ptr<Server> serv,
                       std::shared_ptr<ExperimentInterface> exp)
      : serv_(serv), exp_(exp) {}

  auto AcceptConnection(ConnectionInfo info) {
    auto id = exp_->AddSubject();
    serv_->AddSubject(id, info);
  }

 private:
  std::shared_ptr<Server> serv_;
  std::shared_ptr<ExperimentInterface> exp_;
};

}  // namespace assetmarket

#endif  // CONNECTION_CONTROLLER_HH
