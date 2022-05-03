#include "asio_client.hh"

#include <string>

namespace assetmarket {

auto AsioClient::Connect(const std::string& host, const uint16_t port) -> bool {
  try {
    asio::ip::tcp::resolver resolver(m_context);
    asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve(host, std::to_string(port));

    m_connection = std::make_unique<AsioConnection>(
        m_context, asio::ip::tcp::socket(m_context),
        [this](Message message) { ProcessMessage(message); },
        [this](std::error_code ec) {});

    m_connection->ConnectToServer(endpoints);
    thrContext = std::thread([this]() { m_context.run(); });
  } catch (std::exception& e) {
    return false;
  }
  return true;
}

auto AsioClient::Disconnect() -> void {
  if (IsConnected()) {
    m_connection->Disconnect();
  }
  m_context.stop();
  if (thrContext.joinable()) thrContext.join();
  m_connection.release();
}

auto AsioClient::IsConnected() -> bool {
  if (m_connection)
    return m_connection->IsConnected();
  else
    return false;
}

auto AsioClient::ProcessMessage(Message message) -> void {
  for (auto& proc : processors_) {
    proc->ProcessMessage(message);
  }
}

auto AsioClient::Send(const Message& msg) -> void {
  if (IsConnected()) m_connection->Send(msg);
}

}  // namespace assetmarket
