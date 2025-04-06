#ifndef SERVER_MESSAGE_PROCESSOR_HH
#define SERVER_MESSAGE_PROCESSOR_HH

#include "comms/connection.hh"
#include "comms/connection_info.hh"
#include "comms/message.hh"

namespace exptemplate {

template <typename T>
class ServerMessageProcessor {
 public:
  virtual auto ProcessMessage(comms::Message<T> message) -> void = 0;
  virtual ~ServerMessageProcessor() {}
};

}  // namespace exptemplate

#endif  // SERVER_MESSAGE_PROCESSOR_HH
