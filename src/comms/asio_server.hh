#ifndef ASIO_SERVER_HH
#define ASIO_SERVER_HH

#include <vector>

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <iostream>
#include <string>

#include "../client_message_processor.hh"
#include "comms/asio_connection.hh"
#include "comms/connection.hh"
#include "comms/server.hh"

namespace exptemplate {
namespace comms {

enum class BaseMessageType { INVALID = 0, LOGIN, LOGOUT, DEBUGMESSAGE };

template <typename T>
concept MessageT = std::is_base_of_v<BaseMessageType, T>;

class InvalidClientException : public std::exception {
 public:
  const char* what() const noexcept override {
    return "Invalid client ConnectionID";
  }
};

// The base of this code comes from
// raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/Videos/Networking
template <MessageT T>
class AsioServer : public Server<T> {
 public:
  AsioServer(asio::io_context& context, uint16_t port);
  virtual ~AsioServer() { Stop(); }

  auto Start() -> bool;
  auto Stop() -> void;

  auto StartAcceptingConnections() -> void { accepting_conns_ = true; }
  auto StopAcceptingConnections() -> void { accepting_conns_ = false; }

  auto Send(ConnectionID id, const Message<T>& message) -> void override;
  auto SendAll(const Message<T>& message) -> void override;
  auto SendAdmins(const Message<T>& message) -> void override;
  auto AddProcessor(std::shared_ptr<ClientMessageProcessor<T>> proc) -> void;
  auto ProcessMessage(ConnectionID id, Message<T> message) -> void;
  auto AddConnection(std::shared_ptr<Connection<T>> conn) -> void override;
  auto AddClient(ConnectionID id, const ConnectionInfo<T>& conn)
      -> void override;
  auto AddClient(ConnectionID id, size_t conn_id) -> void override;
  auto AddAdmin(size_t conn_id) -> void override;
  auto ClientCount() const -> size_t { return clients_.size(); }
  auto UnknownConnectionCount() const -> size_t { return connections_.size(); }

 private:
  auto WaitForConnection() -> void;
  auto MakeConnection(size_t id, asio::ip::tcp::socket socket)
      -> std::unique_ptr<AsioConnection<T>>;

  size_t next_id = 0;
  bool accepting_conns_ = true;
  std::vector<ConnectionInfo<T>> connections_;
  std::unordered_map<ConnectionID, ConnectionInfo<T>> clients_;
  std::vector<ConnectionID> client_ids_;
  std::vector<ConnectionInfo<T>> admins_;
  std::vector<std::shared_ptr<ClientMessageProcessor<T>>> processors_;
  asio::io_context& context_;
  asio::ip::tcp::acceptor m_asioAcceptor;
  uint32_t nIDCounter = 10000;
};

template <MessageT T>
struct ClientMessage {
  comms::ConnectionInfo<T>& client;
  comms::Message<T> message;
};

template <MessageT T>
AsioServer<T>::AsioServer(asio::io_context& context, uint16_t port)
    : context_(context),
      m_asioAcceptor(context,
                     asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

template <MessageT T>
auto AsioServer<T>::Start() -> bool {
  try {
    WaitForConnection();
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

template <MessageT T>
auto AsioServer<T>::Stop() -> void {
  accepting_conns_ = false;
  connections_.clear();
  clients_.clear();
  client_ids_.clear();
}

template <MessageT T>
auto AsioServer<T>::Send(ConnectionID id, const Message<T>& message) -> void {
  auto it = clients_.find(id);
  if (it != clients_.end()) {
    it->second.conn->Send(message);
    Message<T> admin_mess = {message.header_.type,
                             message.body_ + " " + std::to_string(id)};
    SendAdmins(admin_mess);
  } else {
    throw comms::InvalidClientException();
  }
}

template <MessageT T>
auto AsioServer<T>::SendAll(const Message<T>& message) -> void {
  for (auto& [key, info] : clients_) {
    info.conn->Send(message);
  }
  SendAdmins(message);
};

template <MessageT T>
auto AsioServer<T>::SendAdmins(const Message<T>& message) -> void {
  for (auto& info : admins_) {
    info.conn->Send(message);
  }
}

template <MessageT T>
auto AsioServer<T>::AddConnection(std::shared_ptr<Connection<T>> conn) -> void {
  connections_.emplace_back(next_id++, "", conn);
}

template <MessageT T>
auto AsioServer<T>::AddProcessor(
    std::shared_ptr<ClientMessageProcessor<T>> proc) -> void {
  processors_.push_back(proc);
}

template <MessageT T>
auto AsioServer<T>::ProcessMessage(ConnectionID id, Message<T> message)
    -> void {
  if (std::find(client_ids_.begin(), client_ids_.end(), id) !=
      client_ids_.end()) {
    for (auto& proc : processors_) {
      proc->ProcessClientMessage(id, message);
    }
  } else {
    for (auto& proc : processors_) {
      proc->ProcessMessage(id, message);
    }
  }
}

template <MessageT T>
auto AsioServer<T>::AddClient(ConnectionID id, const ConnectionInfo<T>& conn)
    -> void {
  clients_.emplace(id, conn);
  std::erase_if(connections_,
                [&conn](ConnectionInfo<T> c) { return c.id == conn.id; });
}

template <MessageT T>
auto AsioServer<T>::AddAdmin(size_t conn_id) -> void {
  auto conn =
      std::find_if(connections_.begin(), connections_.end(),
                   [conn_id](ConnectionInfo<T> c) { return c.id == conn_id; });
  admins_.push_back(*conn);
  std::erase_if(connections_,
                [&conn](ConnectionInfo<T> c) { return c.id == conn->id; });
}

template <MessageT T>
auto AsioServer<T>::AddClient(ConnectionID id, size_t conn_id) -> void {
  auto conn =
      std::find_if(connections_.begin(), connections_.end(),
                   [conn_id](ConnectionInfo<T> c) { return c.id == conn_id; });

  AddClient(id, *conn);
}

template <MessageT T>
auto AsioServer<T>::WaitForConnection() -> void {
  m_asioAcceptor.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
          auto id = next_id++;
          auto ip = socket.remote_endpoint().address().to_string();
          std::shared_ptr<AsioConnection<T>> conn =
              MakeConnection(id, std::move(socket));
          if (accepting_conns_) {
            connections_.emplace_back(id, ip, conn);
            conn->ConnectToClient();
          } else {
            std::cerr << "rejected connection\n";
          }
        } else {
          std::cerr << "failed new connection:" << ec.message() << "\n";
        }
        WaitForConnection();
      });
}

template <MessageT T>
auto AsioServer<T>::MakeConnection(size_t id, asio::ip::tcp::socket socket)
    -> std::unique_ptr<AsioConnection<T>> {
  auto newconn = std::make_unique<AsioConnection<T>>(
      context_, std::move(socket),
      [this, id](Message<T> message) { ProcessMessage(id, message); },
      [id](std::error_code ec) {
        std::cerr << "failed message:" << ec.message() << "\n";
      });
  return newconn;
}

}  // namespace comms
}  // namespace exptemplate
#endif  // ASIO_SERVER_HH
