#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

#include "comms/asio_server.hh"
#include "comms/connection.hh"
#include "communication_test_consts.hh"
#include "test_client.hh"

using namespace ::testing;
using namespace exptemplate;
using namespace commstest;
using namespace exptemplate::comms;

class MockConnection : public Connection<TestMessage> {
 public:
  auto Send(const exptemplate::comms::Message<TestMessage>&) -> void override {
    n_mess_ += 1;
  }

  int n_mess_ = 0;
};
auto MakeMockConnection() -> std::shared_ptr<MockConnection> {
  return std::make_shared<MockConnection>();
}

class MockProcessor : public ClientMessageProcessor<TestMessage> {
 public:
  auto ProcessClientMessage(ConnectionID id,
                            comms::Message<TestMessage> message)
      -> void override {}
  auto ProcessMessage(size_t, exptemplate::comms::Message<TestMessage>)
      -> void override {
    n_proc_ += 1;
  }
  int n_proc_ = 0;
};

class AnAsioServer : public Test {
 public:
  AnAsioServer() : context_(), serv_(context_, 12345) { serv_.Start(); }
  auto AddConnections(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
      serv_.AddConnection(TEST_CLIENTS[i]);
    }
  }
  auto AddClients(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
      serv_.AddClient(i, {i, "", TEST_CLIENTS[i]});
    }
  }
  asio::io_context context_;
  std::vector<std::shared_ptr<MockConnection>> TEST_CLIENTS{
      MakeMockConnection(), MakeMockConnection(), MakeMockConnection(),
      MakeMockConnection(), MakeMockConnection()};
  AsioServer<TestMessage> serv_;
};

TEST_F(AnAsioServer, CanSendMessagesToOneClient) {
  auto conn = MakeMockConnection();
  auto conn2 = MakeMockConnection();
  ConnectionInfo<TestMessage> info{0, "", conn};
  ConnectionInfo<TestMessage> info2{0, "", conn2};
  serv_.AddClient(2, info);
  serv_.AddClient(1, info2);
  serv_.Send(2, TEST_MESSAGE);
  ASSERT_THAT(conn->n_mess_, Eq(1));
  ASSERT_THAT(conn2->n_mess_, Eq(0));
}

TEST_F(AnAsioServer, ThrowsIfSendingToInvalidClient) {
  auto conn = MakeMockConnection();
  ASSERT_THROW(serv_.Send(2, TEST_MESSAGE), comms::InvalidClientException);
  serv_.AddClient(2, {0, "", conn});
  ASSERT_THROW(serv_.Send(0, TEST_MESSAGE), comms::InvalidClientException);
}

TEST_F(AnAsioServer, HasNoClientsOnStart) {
  ASSERT_THAT(serv_.ClientCount(), Eq(0));
}

TEST_F(AnAsioServer, ClientsCountedTowardsConnections) {
  AddClients(3);
  ASSERT_THAT(serv_.ClientCount(), Eq(3));
}

TEST_F(AnAsioServer, SendsMessagesToAllClients) {
  AddClients(3);
  serv_.SendAll(TEST_MESSAGE);
  for (unsigned int i = 0; i < 3; ++i) {
    ASSERT_THAT(TEST_CLIENTS[i]->n_mess_, Eq(1));
  }
}

TEST_F(AnAsioServer, DoesNotSendToUnknownConnections) {
  AddClients(3);
  serv_.AddConnection(TEST_CLIENTS[3]);
  serv_.SendAll(TEST_MESSAGE);
  for (unsigned int i = 0; i < 3; ++i) {
    ASSERT_THAT(TEST_CLIENTS[i]->n_mess_, Eq(1));
  }
  ASSERT_THAT(TEST_CLIENTS[3]->n_mess_, Eq(0));
}

TEST_F(AnAsioServer, CanAddProcessor) {
  auto proc = std::make_shared<MockProcessor>();
  serv_.AddProcessor(proc);
  serv_.ProcessMessage(0, TEST_MESSAGE);
  serv_.ProcessMessage(0, TEST_MESSAGE);
  ASSERT_THAT(proc->n_proc_, Eq(2));
}

TEST_F(AnAsioServer, AllProcessorsSeeMessage) {
  auto proc = std::make_shared<MockProcessor>();
  auto proc2 = std::make_shared<MockProcessor>();
  serv_.AddProcessor(proc);
  serv_.AddProcessor(proc2);
  serv_.ProcessMessage(0, TEST_MESSAGE);
  serv_.ProcessMessage(0, TEST_MESSAGE);
  ASSERT_THAT(proc->n_proc_, Eq(2));
  ASSERT_THAT(proc2->n_proc_, Eq(2));
}

TEST_F(AnAsioServer, CanStopAcceptingClients) {
  TestClient client;
  serv_.StopAcceptingConnections();
  auto finished = client.WaitForConnect();
  ASSERT_TRUE(finished);
  finished = client.WaitReceiveMessage();
  ASSERT_TRUE(finished);
  ASSERT_TRUE(client.failed);
  ASSERT_THAT(serv_.UnknownConnectionCount(), Eq(0));
}

TEST_F(AnAsioServer, AcceptsClients) {
  TestClient client;
  auto finished = client.WaitForConnect();
  ASSERT_TRUE(finished);
  ASSERT_TRUE(client.received_message);
  ASSERT_THAT(serv_.UnknownConnectionCount(), Eq(1));
  // client.received_message = false;
  // serv_.SendAll(TEST_MESSAGE);
  // finished = client.WaitReceiveMessage();
  // ASSERT_TRUE(finished);
  // ASSERT_TRUE(client.received_message);
  serv_.Stop();
}

TEST_F(AnAsioServer, SendsMessagesToAdminClients) {
  auto admin_conn = MakeMockConnection();
  auto regular_conn = MakeMockConnection();
  serv_.AddConnection(admin_conn);
  serv_.AddConnection(regular_conn);
  serv_.AddAdmin(0);
  serv_.SendAdmins(TEST_MESSAGE);
  ASSERT_THAT(admin_conn->n_mess_, Eq(1));
  ASSERT_THAT(regular_conn->n_mess_, Eq(0));
}
