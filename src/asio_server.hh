#ifndef ASIO_SERVER_HH
#define ASIO_SERVER_HH

#include <vector>

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "asio_connection.hh"
#include "asset_market_enums.hh"
#include "client_message_processor.hh"
#include "connection.hh"
#include "server.hh"
#include "subject.hh"

namespace assetmarket {

// The base of this code comes from
// raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine/master/Videos/Networking
class AsioServer : public Server {
 public:
  AsioServer(asio::io_context& context, uint16_t port);
  virtual ~AsioServer() { Stop(); }

  auto Start() -> bool;
  auto Stop() -> void;

  auto AddConnection(std::shared_ptr<Connection> conn) -> void;
  auto StartAcceptingClients() -> void { accepting_clients_ = true; }
  auto StopAcceptingClients() -> void { accepting_clients_ = false; }

  auto Send(SubjectID id, const Message& message) -> void override;
  auto SendAll(const Message& message) -> void override;
  auto SendAdmins(const Message& message) -> void override;
  auto AddProcessor(std::shared_ptr<ClientMessageProcessor> proc) -> void;
  auto ProcessMessage(size_t id, Message message) -> void;
  auto AddSubject(SubjectID id, const ConnectionInfo& conn) -> void override;
  auto AddSubject(SubjectID id, size_t conn_id) -> void override;
  auto AddAdmin(size_t conn_id) -> void override;
  auto SubjectCount() const -> size_t { return subjects_.size(); }

  auto UnknownConnectionCount() const -> size_t { return connections_.size(); }

 private:
  auto WaitForClientConnection() -> void;
  auto MakeConnection(size_t id, asio::ip::tcp::socket socket)
      -> std::unique_ptr<AsioConnection>;

  size_t next_id = 0;
  bool accepting_clients_ = true;
  std::vector<ConnectionInfo> connections_;
  std::unordered_map<SubjectID, ConnectionInfo> subjects_;
  std::unordered_map<size_t, SubjectID> subject_ids_;
  std::vector<ConnectionInfo> admins_;
  std::vector<std::shared_ptr<ClientMessageProcessor>> processors_;
  asio::io_context& context_;
  asio::ip::tcp::acceptor m_asioAcceptor;
  uint32_t nIDCounter = 10000;
};

}  // namespace assetmarket
#endif  // ASIO_SERVER_HH
