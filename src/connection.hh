#ifndef CONNECTION_HH
#define CONNECTION_HH

#include "message.hh"

namespace assetmarket {
class Connection {
 public:
  virtual auto Send(const Message& message) -> void = 0;
  virtual ~Connection() {}
};
}  // namespace assetmarket
#endif  // CONNECTION_HH
