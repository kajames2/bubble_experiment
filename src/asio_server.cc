#include "asio_server.hh"

#include <iostream>

namespace assetmarket {
AsioServer::AsioServer(uint16_t port)
    : m_asioAcceptor(context_,
                     asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

auto AsioServer::Start() -> bool {
  try {
    WaitForClientConnection();
    m_threadContext = std::thread([this]() { context_.run(); });
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

auto AsioServer::Stop() -> void {
  context_.stop();
  if (m_threadContext.joinable()) m_threadContext.join();
}

auto AsioServer::Send(size_t id, const Message& message) -> void {
  connections_[id]->Send(message);
};

auto AsioServer::SendAll(const Message& message) -> void {
  for (auto& client : connections_) {
    client->Send(message);
  }
};

auto AsioServer::AddConnection(std::shared_ptr<Connection> conn) -> void {
  connections_.push_back(conn);
}

auto AsioServer::AddProcessor(std::shared_ptr<ClientMessageProcessor> proc)
    -> void {
  processors_.push_back(proc);
}

auto AsioServer::ProcessMessage(size_t id, Message message) -> void {
  for (auto& proc : processors_) {
    proc->ProcessMessage(id, message);
  }
}

auto AsioServer::WaitForClientConnection() -> void {
  m_asioAcceptor.async_accept([this](std::error_code ec,
                                     asio::ip::tcp::socket socket) {
    if (!ec) {
      std::shared_ptr<AsioConnection> conn = MakeConnection(std::move(socket));
      if (accepting_clients_) {
        connections_.push_back(conn);
        conn->ConnectToClient();
        conn->Send(Message(MessageType::Connected));
      } else {
        std::cerr << "rejected connection\n";
      }
    } else {
      std::cerr << "failed new connection:" << ec.message() << "\n";
    }
    WaitForClientConnection();
  });
}

auto AsioServer::MakeConnection(asio::ip::tcp::socket socket)
    -> std::unique_ptr<AsioConnection> {
  auto id = connections_.size();
  auto newconn = std::make_unique<AsioConnection>(
      context_, std::move(socket),
      [this, id](Message message) { ProcessMessage(id, message); },
      [id](std::error_code ec) {
        std::cerr << "failed message:" << ec.message() << "\n";
      });
  return newconn;
}

}  // namespace assetmarket
