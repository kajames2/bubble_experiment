#ifndef CLIENT_MESSAGE_PROCESSOR_HH
#define CLIENT_MESSAGE_PROCESSOR_HH

#include "connection.hh"
#include "connection_info.hh"
#include "message.hh"

namespace assetmarket {

struct ClientMessage {
  ConnectionInfo& client;
  Message message;
};

class ClientMessageProcessor {
 public:
  virtual auto ProcessMessage(size_t id, Message message) -> void = 0;
  virtual ~ClientMessageProcessor() {}
};

}  // namespace assetmarket

#endif  // CLIENT_MESSAGE_PROCESSOR_HH
