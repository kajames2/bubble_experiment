#ifndef ASIO_CLIENT_HH
#define ASIO_CLIENT_HH

#include <string>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <vector>

#include "asio_connection.hh"
#include "server_message_processor.hh"

// The base of this code comes from
// raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/Videos/Networking
namespace assetmarket {

class AsioClient {
 public:
  AsioClient() {}

  virtual ~AsioClient() { Disconnect(); }

 public:
  auto Connect(const std::string& host, const uint16_t port) -> bool;
  auto Disconnect() -> void;
  auto IsConnected() -> bool;
  auto ProcessMessage(Message message) -> void;
  auto Send(const Message& msg) -> void;
  auto AddProcessor(std::shared_ptr<ServerMessageProcessor> proc) -> void {
    processors_.push_back(proc);
  }

 private:
  asio::io_context m_context;
  std::thread thrContext;
  std::unique_ptr<AsioConnection> m_connection;
  std::vector<std::shared_ptr<ServerMessageProcessor>> processors_;
};

}  // namespace assetmarket
#endif  // ASIO_CLIENT_HH
