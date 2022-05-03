#ifndef CONNECTION_INFO_HH
#define CONNECTION_INFO_HH

#include <memory>

#include "connection.hh"
#include <string>

namespace assetmarket {
struct ConnectionInfo {
  size_t id;
  std::string ip;
  std::shared_ptr<Connection> conn;
};
}  // namespace assetmarket

#endif  // CONNECTION_INFO_HH
