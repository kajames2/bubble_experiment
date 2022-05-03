#ifndef SERVER_HH
#define SERVER_HH

#include "client_message_processor.hh"
#include "connection_info.hh"
#include "message.hh"
#include "subject.hh"

namespace assetmarket {
class Server {
 public:
  virtual auto Send(SubjectID id, const Message& message) -> void = 0;
  virtual auto SendAll(const Message& message) -> void = 0;
  virtual auto AddSubject(SubjectID id, const ConnectionInfo& conn) -> void = 0;
  virtual ~Server() {}
};
}  // namespace assetmarket
#endif  // SERVER_HH
