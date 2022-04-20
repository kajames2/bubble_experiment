#ifndef SERVER_HH
#define SERVER_HH

#include "client_message_processor.hh"
#include "message.hh"

namespace assetmarket {
class Server {
 public:
  virtual auto Send(size_t id, const Message& message) -> void = 0;
  virtual auto SendAll(const Message& message) -> void = 0;
  virtual ~Server() {}
};
}  // namespace assetmarket
#endif  // SERVER_HH
