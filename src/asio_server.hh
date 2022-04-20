#ifndef ASIO_SERVER_HH
#define ASIO_SERVER_HH

#include <iostream>
#include <vector>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "asio_connection.hh"
#include "client_message_processor.hh"
#include "connection.hh"
#include "server.hh"

namespace assetmarket {

// The base of this code comes from
// raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/Videos/Networking
class AsioServer : public Server {
 public:
  AsioServer(uint16_t port, std::shared_ptr<ClientMessageProcessor> proc)
      : processor_(proc),
        m_asioAcceptor(contex_,
                       asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

  virtual ~AsioServer() { Stop(); }

  bool Start() {
    try {
      WaitForClientConnection();
      m_threadContext = std::thread([this]() { contex_.run(); });
    } catch (std::exception& e) {
      std::cerr << "[SERVER] Exception: " << e.what() << "\n";
      return false;
    }
    std::cout << "[SERVER] Started!\n";
    return true;
  }

  auto Send(size_t id, const Message& message) -> void override {
    clients_[id]->Send(message);
  };

  auto SendAll(const Message& message) -> void override {
    for (auto& client : clients_) {
      client->Send(message);
    }
  };
  auto AddConnection(std::shared_ptr<Connection> conn) -> void {
    clients_.push_back(conn);
  }

  auto ProcessMessage(int id, Message message) -> void {
    processor_->ProcessMessage(id, message);
  }

  void Stop() {
    contex_.stop();
    if (m_threadContext.joinable()) m_threadContext.join();
    std::cout << "[SERVER] Stopped!\n";
  }

  void WaitForClientConnection() {
    m_asioAcceptor.async_accept([this](std::error_code ec,
                                       asio::ip::tcp::socket socket) {
      if (!ec) {
        std::cout << "[SERVER] New Connection: " << socket.remote_endpoint()
                  << "\n";

        int id = clients_.size();
        std::shared_ptr<AsioConnection> newconn =
            std::make_shared<AsioConnection>(
                contex_, std::move(socket),
                [this, id](Message message) { ProcessMessage(id, message); },
                [this, id](asio::error_code ec) {
                  std::cerr << "[SERVER] Connection Error " << id << ": "
                            << ec.message() << "\n";
                });
        clients_.push_back(newconn);
        newconn->ConnectToClient();
      } else {
        std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
      }

      WaitForClientConnection();
    });
  }

 private:
  std::vector<std::shared_ptr<Connection>> clients_;
  std::shared_ptr<ClientMessageProcessor> processor_;
  asio::io_context contex_;
  std::thread m_threadContext;
  asio::ip::tcp::acceptor m_asioAcceptor;
  uint32_t nIDCounter = 10000;
};

}  // namespace assetmarket
#endif  // ASIO_SERVER_HH
