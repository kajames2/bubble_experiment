#ifndef CONNECTION_INFO_HH
#define CONNECTION_INFO_HH

#include <memory>

#include "connection.hh"

namespace assetmarket {
struct ConnectionInfo {
  std::shared_ptr<Connection> conn;
};
}  // namespace assetmarket

#endif  // CONNECTION_INFO_HH
