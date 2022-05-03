#ifndef MESSAGE_DIRECTOR
#define MESSAGE_DIRECTOR

#include <sstream>

#include "client_controller.hh"
#include "client_message_processor.hh"
#include "connection_controller.hh"
#include "experiment_controller.hh"
#include "message.hh"

namespace assetmarket {
class MessageDirector : public ClientMessageProcessor {
 public:
  MessageDirector(std::shared_ptr<ClientController> subject_cont,
                  std::shared_ptr<ExperimentController> exp_cont,
                  std::shared_ptr<ConnectionController> conn_cont)
      : subject_cont_(subject_cont),
        exp_cont_(exp_cont),
        conn_cont_(conn_cont) {}

  auto ProcessMessage(size_t id, Message message) -> void override {
    switch (message.header_.id) {
      case MessageType::ClientType:
        int cint;
        std::stringstream ss(message.body_);
        ss >> cint;
        ClientType c = static_cast<ClientType>(cint);
        if (c == ClientType::Subject) {
          conn_cont_->AcceptSubject(id);
        }
        break;
    }
  }

 private:
  std::shared_ptr<ClientController> subject_cont_;
  std::shared_ptr<ExperimentController> exp_cont_;
  std::shared_ptr<ConnectionController> conn_cont_;
};
}  // namespace assetmarket

#endif  // MESSAGE_DIRECTOR
