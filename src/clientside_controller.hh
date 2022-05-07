#ifndef CLIENTSIDE_CONTROLLER_HH
#define CLIENTSIDE_CONTROLLER_HH

#include <iostream>
#include <memory>
#include <string>

#include "asio_client.hh"
#include "asset_market_enums.hh"
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
        break;
      default:
        std::cerr << message.header_ << "---" << message.body_ << std::endl;
        break;
    }
  }

 private:
  std::shared_ptr<AsioClient> client_;
};

}  // namespace assetmarket
#endif  // CLIENTSIDE_CONTROLLER_HH
