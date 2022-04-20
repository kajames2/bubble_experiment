#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

#include "asio_server.hh"
#include "connection.hh"

using namespace ::testing;
using namespace assetmarket;

class MockConnection : public Connection {
 public:
  auto Send(const assetmarket::Message& message) -> void override {
    n_mess_ += 1;
  }

  int n_mess_ = 0;
};

class MockProcessor : public ClientMessageProcessor {
 public:
  auto ProcessMessage(int id, assetmarket::Message message) -> void override {
    n_proc_ += 1;
  }
  int n_proc_ = 0;
};

static const assetmarket::Message TEST_MESSAGE(MessageType::DebugMessage, "");
class AnAsioServer : public Test {
 public:
  AnAsioServer()
      : processor_(std::make_shared<MockProcessor>()),
        serv_(12345, processor_) {
    for (int i = 0; i < 5; ++i) {
      clients_.push_back(std::make_shared<MockConnection>());
      serv_.AddConnection(clients_.back());
    }
  }
  std::vector<std::shared_ptr<MockConnection>> clients_;
  std::shared_ptr<MockProcessor> processor_;
  AsioServer serv_;
};

TEST_F(AnAsioServer, CanSendSingleMessage) {
  serv_.Send(0, TEST_MESSAGE);
  ASSERT_THAT(clients_[0]->n_mess_, Eq(1));
}

TEST_F(AnAsioServer, CanSendAllMessage) {
  serv_.SendAll(TEST_MESSAGE);
  for (const auto& client : clients_) {
    ASSERT_THAT(client->n_mess_, Eq(1));
  }
}

TEST_F(AnAsioServer, PassesOnToProcessMessages) {
  serv_.ProcessMessage(0, TEST_MESSAGE);
  serv_.ProcessMessage(0, TEST_MESSAGE);
  ASSERT_THAT(processor_->n_proc_, Eq(2));
}
