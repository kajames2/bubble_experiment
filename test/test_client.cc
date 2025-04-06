#include "test_client.hh"

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

#include "comms/asio_connection.hh"

using namespace std::chrono_literals;
using namespace exptemplate;
using namespace exptemplate::comms;
using namespace commstest;

TestClient::TestClient() : context_() {
  asio::ip::tcp::socket sock(context_);
  conn = std::make_unique<AsioConnection<TestMessage>>(
      context_, std::move(sock),
      [this](Message<TestMessage> mess) {
        received_message = true;
        v.notify_all();
      },
      [&](std::error_code ec) {
        failed = true;
        v.notify_all();
      },
      [this](std::error_code ec) {
        if (!ec) {
          connected = true;
        } else {
          failed = true;
        }
        v.notify_all();
      });
}

auto TestClient::WaitForConnect() -> bool {
  asio::ip::tcp::resolver resolver(context_);
  endpoints = asio::ip::tcp::resolver::results_type(
      resolver.resolve("localhost", std::to_string(12345)));
  conn->ConnectToServer(endpoints);
  thd = std::make_unique<std::thread>([&]() { context_.run(); });
  auto now = std::chrono::system_clock::now();
  std::unique_lock<std::mutex> lock(m);
  bool finished = v.wait_until(lock, now + 2s, [&]() {
    return static_cast<bool>(received_message || failed);
  });
  return finished;
}

auto TestClient::WaitReceiveMessage() -> bool {
  auto now = std::chrono::system_clock::now();
  std::unique_lock<std::mutex> lock(m);
  if (received_message) return true;
  bool finished = v.wait_until(lock, now + 2s, [&]() {
    return static_cast<bool>(received_message || failed);
  });
  return finished || received_message;
}

TestClient::~TestClient() {
  if (conn && conn->IsConnected()) {
    conn->Disconnect();
  }
  context_.stop();
  if (thd->joinable()) thd->join();
  conn.release();
}
