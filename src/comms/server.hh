#ifndef SERVER_HH
#define SERVER_HH

#include "connection.hh"
#include "message.hh"

namespace exptemplate {
namespace comms {
template <typename T>
class Server {
 public:
  virtual auto Send(ConnectionID id, const Message<T>& message) -> void = 0;
  virtual auto SendAll(const Message<T>& message) -> void = 0;
  virtual auto SendAdmins(const Message<T>& message) -> void = 0;
  virtual auto AddConnection(std::shared_ptr<Connection<T>> conn) -> void = 0;
  virtual auto AddClient(ConnectionID id, const ConnectionInfo<T>& conn)
      -> void = 0;
  virtual auto AddClient(ConnectionID id, size_t conn_id) -> void = 0;
  virtual auto AddAdmin(size_t conn_id) -> void = 0;
  virtual ~Server() {}
};
}  // namespace comms
}  // namespace exptemplate
#endif  // SERVER_HH
