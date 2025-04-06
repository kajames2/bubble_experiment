#ifndef TEST_CONNECTION_HH
#define TEST_CONNECTION_HH

#include <atomic>
#include <chrono>
#include <thread>

#include "comms/asio_connection.hh"
#include "communication_test_consts.hh"

namespace commstest {

class TestClient {
 public:
  TestClient();
  auto WaitForConnect() -> bool;
  auto WaitReceiveMessage() -> bool;
  ~TestClient();

  std::atomic<bool> received_message = false;
  std::atomic<bool> connected = false;
  std::atomic<bool> failed = false;
  std::mutex m;
  std::condition_variable v;
  bool error_code = false;
  asio::io_context context_;
  std::unique_ptr<std::thread> thd;
  std::unique_ptr<exptemplate::comms::AsioConnection<TestMessage>> conn;
  asio::ip::tcp::resolver::results_type endpoints;
};
}  // namespace commstest
#endif  // TEST_CONNECTION_HH
