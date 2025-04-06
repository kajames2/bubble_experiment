#ifndef ASIO_CLIENT_HH
#define ASIO_CLIENT_HH

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <string>
#include <vector>

#include "comms/asio_connection.hh"
#include "server_message_processor.hh"

// The base of this code comes from
// raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/Videos/Networking
namespace exptemplate {
namespace comms {

template <typename T>
class AsioClient {
 public:
  AsioClient(asio::io_context& context) : context_(context) {}

  virtual ~AsioClient() { Disconnect(); }

 public:
  auto Connect(const std::string& host, const uint16_t port) -> bool;
  auto Disconnect() -> void;
  auto IsConnected() -> bool;
  auto ProcessMessage(Message<T> message) -> void;
  auto Send(const Message<T>& msg) -> void;
  auto AddProcessor(std::shared_ptr<ServerMessageProcessor<T>> proc) -> void {
    processors_.push_back(proc);
  }

 private:
  asio::io_context& context_;
  std::unique_ptr<AsioConnection<T>> m_connection;
  std::vector<std::shared_ptr<ServerMessageProcessor<T>>> processors_;
};

template <typename T>
auto AsioClient<T>::Connect(const std::string& host, const uint16_t port)
    -> bool {
  try {
    asio::ip::tcp::resolver resolver(context_);
    asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve(host, std::to_string(port));

    m_connection = std::make_unique<AsioConnection>(
        context_, asio::ip::tcp::socket(context_),
        [this](Message<T> message) { ProcessMessage(message); },
        [this](std::error_code ec) {});

    m_connection->ConnectToServer(endpoints);
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

template <typename T>
auto AsioClient<T>::Disconnect() -> void {
  if (IsConnected()) {
    m_connection->Disconnect();
  }
  m_connection.release();
}

template <typename T>
auto AsioClient<T>::IsConnected() -> bool {
  if (m_connection)
    return m_connection->IsConnected();
  else
    return false;
}

template <typename T>
auto AsioClient<T>::ProcessMessage(Message<T> message) -> void {
  for (auto& proc : processors_) {
    proc->ProcessMessage(message);
  }
}

template <typename T>
auto AsioClient<T>::Send(const Message<T>& msg) -> void {
  if (IsConnected()) m_connection->Send(msg);
}

}  // namespace comms
}  // namespace exptemplate
#endif  // ASIO_CLIENT_HH
