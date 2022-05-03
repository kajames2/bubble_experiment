#ifndef CLIENTSIDE_CONTROLLER_HH
#define CLIENTSIDE_CONTROLLER_HH

#include <iostream>
#include <memory>
#include <string>

#include "asio_server.hh"
#include "message.hh"
#include "server_message_processor.hh"

namespace assetmarket {
class ClientsideController : public ServerMessageProcessor {
 public:
  ClientsideController(std::shared_ptr<AsioClient> client) : client_(client) {}
  auto ProcessMessage(Message message) -> void override {
    switch (message.header_.id) {
      case MessageType::Connected:
        client_->Send(
            Message(MessageType::ClientType,
                    std::to_string(static_cast<int>(ClientType::Subject))));
        break;
      case MessageType::DebugMessage:
        std::cerr << message.body_ << std::endl;
      default:
        break;
    }
  }

 private:
  std::shared_ptr<AsioClient> client_;
};

}  // namespace assetmarket
#endif  // CLIENTSIDE_CONTROLLER_HH
