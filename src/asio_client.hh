#ifndef ASIO_CLIENT_HH
#define ASIO_CLIENT_HH

#include <iostream>
#include <string>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "asio_connection.hh"

// The base of this code comes from
// raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/Videos/Networking
namespace assetmarket {

class AsioClient {
 public:
  AsioClient() {}

  virtual ~AsioClient() { Disconnect(); }

 public:
  bool Connect(const std::string& host, const uint16_t port) {
    try {
      asio::ip::tcp::resolver resolver(m_context);
      asio::ip::tcp::resolver::results_type endpoints =
          resolver.resolve(host, std::to_string(port));

      m_connection = std::make_unique<AsioConnection>(
          m_context, asio::ip::tcp::socket(m_context),
          [this](Message message) {
            std::cout << message.header_ << " " << message.body_ << "\n";
          },
          [this](asio::error_code ec) {
            std::cerr << "Connection Error: " << ec.message() << "\n";
          });

      m_connection->ConnectToServer(endpoints);
      thrContext = std::thread([this]() { m_context.run(); });
    } catch (std::exception& e) {
      std::cerr << "Client Exception: " << e.what() << "\n";
      return false;
    }
    return true;
  }

  void Disconnect() {
    if (IsConnected()) {
      m_connection->Disconnect();
    }
    m_context.stop();
    if (thrContext.joinable()) thrContext.join();
    m_connection.release();
  }

  bool IsConnected() {
    if (m_connection)
      return m_connection->IsConnected();
    else
      return false;
  }

 public:
  void Send(const Message& msg) {
    if (IsConnected()) m_connection->Send(msg);
  }

 protected:
  asio::io_context m_context;
  std::thread thrContext;
  std::unique_ptr<AsioConnection> m_connection;
};
}  // namespace assetmarket
#endif  // ASIO_CLIENT_HH
