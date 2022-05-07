#ifndef ADMINSIDE_CONTROLLER_HH
#define ADMINSIDE_CONTROLLER_HH

#include <iostream>
#include <memory>
#include <string>

#include "asio_client.hh"
#include "asset_market_enums.hh"
#include "message.hh"
#include "server_message_processor.hh"

namespace assetmarket {
class AdminsideController : public ServerMessageProcessor {
 public:
  AdminsideController(std::shared_ptr<AsioClient> admin) : admin_(admin) {}
  auto ProcessMessage(Message message) -> void override {
    switch (message.header_.id) {
      case MessageType::Connected:
        admin_->Send(
            Message(MessageType::ClientType,
                    std::to_string(static_cast<int>(ClientType::Admin))));
        break;
      default:
        std::cerr << message.header_ << "---" << message.body_ << std::endl;
        break;
    }
  }

 private:
  std::shared_ptr<AsioClient> admin_;
};

}  // namespace assetmarket
#endif  // ADMINSIDE_CONTROLLER_HH
