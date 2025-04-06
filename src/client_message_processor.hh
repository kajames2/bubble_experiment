#ifndef CLIENT_MESSAGE_PROCESSOR_HH
#define CLIENT_MESSAGE_PROCESSOR_HH

#include "comms/connection.hh"
#include "comms/message.hh"
#include "subject.hh"

namespace exptemplate {

template <typename T>
class ClientMessageProcessor {
 public:
  virtual auto ProcessClientMessage(SubjectID id, comms::Message<T> message)
      -> void = 0;
  virtual auto ProcessMessage(size_t conn_id, comms::Message<T> message)
      -> void = 0;
  virtual ~ClientMessageProcessor() {}
};

}  // namespace exptemplate

#endif  // CLIENT_MESSAGE_PROCESSOR_HH
