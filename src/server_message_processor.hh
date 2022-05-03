#ifndef SERVER_MESSAGE_PROCESSOR_HH
#define SERVER_MESSAGE_PROCESSOR_HH

#include "connection.hh"
#include "connection_info.hh"
#include "message.hh"

namespace assetmarket {

class ServerMessageProcessor {
 public:
  virtual auto ProcessMessage(Message message) -> void = 0;
  virtual ~ServerMessageProcessor() {}
};

}  // namespace assetmarket

#endif  // SERVER_MESSAGE_PROCESSOR_HH
