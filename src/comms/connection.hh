#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <memory>
#include <string>

#include "message.hh"

namespace exptemplate {
namespace comms {

template <typename T>
class Connection {
 public:
  virtual auto Send(const Message<T>& message) -> void = 0;
  virtual ~Connection() {}
};

using ConnectionID = std::size_t;
template <typename T>
struct ConnectionInfo {
  size_t id;
  std::string ip;
  std::shared_ptr<Connection<T>> conn;
};

}  // namespace comms
}  // namespace exptemplate
#endif  // CONNECTION_HH
