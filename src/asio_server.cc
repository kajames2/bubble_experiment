#include "asio_server.hh"

#include <iostream>
#include <string>

namespace assetmarket {
AsioServer::AsioServer(asio::io_context& context, uint16_t port)
    : context_(context),
      m_asioAcceptor(context,
                     asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

auto AsioServer::Start() -> bool {
  try {
    WaitForClientConnection();
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

auto AsioServer::Stop() -> void { accepting_clients_ = false; }

auto AsioServer::Send(size_t id, const Message& message) -> void {
  subjects_.at(id).conn->Send(message);
  SendAdmins(message);
};

auto AsioServer::SendAll(const Message& message) -> void {
  for (auto& [key, info] : subjects_) {
    info.conn->Send(message);
  }
  SendAdmins(message);
};

auto AsioServer::SendAdmins(const Message& message) -> void {
  for (auto& info : admins_) {
    info.conn->Send(message);
  }
}

auto AsioServer::AddConnection(std::shared_ptr<Connection> conn) -> void {
  connections_.emplace_back(next_id++, "", conn);
}

auto AsioServer::AddProcessor(std::shared_ptr<ClientMessageProcessor> proc)
    -> void {
  processors_.push_back(proc);
}

auto AsioServer::ProcessMessage(size_t id, Message message) -> void {
  if (subject_ids_.contains(id)) {
    auto sub_id = subject_ids_.at(id);
    for (auto& proc : processors_) {
      proc->ProcessSubjectMessage(sub_id, message);
    }
  } else {
    for (auto& proc : processors_) {
      proc->ProcessMessage(id, message);
    }
  }
}

auto AsioServer::AddSubject(SubjectID id, const ConnectionInfo& conn) -> void {
  subjects_.emplace(id, conn);
  subject_ids_.emplace(conn.id, id);
  std::erase_if(connections_,
                [&conn](ConnectionInfo c) { return c.id == conn.id; });
  std::cout << "Added Subject" << std::endl;
}

auto AsioServer::AddAdmin(size_t conn_id) -> void {
  auto conn =
      std::find_if(connections_.begin(), connections_.end(),
                   [conn_id](ConnectionInfo c) { return c.id == conn_id; });
  admins_.push_back(*conn);
  std::erase_if(connections_,
                [&conn](ConnectionInfo c) { return c.id == conn->id; });
}

auto AsioServer::AddSubject(SubjectID id, size_t conn_id) -> void {
  std::cout << "adding subject: " << id << ", " << conn_id << std::endl;
  auto conn =
      std::find_if(connections_.begin(), connections_.end(),
                   [conn_id](ConnectionInfo c) { return c.id == conn_id; });
  AddSubject(id, *conn);
}

auto AsioServer::WaitForClientConnection() -> void {
  m_asioAcceptor.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
          auto id = next_id++;
          auto ip = socket.remote_endpoint().address().to_string();
          std::shared_ptr<AsioConnection> conn =
              MakeConnection(id, std::move(socket));
          if (accepting_clients_) {
            connections_.emplace_back(id, ip, conn);
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

auto AsioServer::MakeConnection(size_t id, asio::ip::tcp::socket socket)
    -> std::unique_ptr<AsioConnection> {
  auto newconn = std::make_unique<AsioConnection>(
      context_, std::move(socket),
      [this, id](Message message) { ProcessMessage(id, message); },
      [id](std::error_code ec) {
        std::cerr << "failed message:" << ec.message() << "\n";
      });
  return newconn;
}

}  // namespace assetmarket
