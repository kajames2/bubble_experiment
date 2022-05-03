#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

#include "asio_server.hh"
#include "communication_test_consts.hh"
#include "connection.hh"
#include "test_client.hh"

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

class MockConnection : public Connection {
 public:
  auto Send(const assetmarket::Message&) -> void override { n_mess_ += 1; }

  int n_mess_ = 0;
};
auto MakeMockConnection() -> std::shared_ptr<MockConnection> {
  return std::make_shared<MockConnection>();
}

class MockProcessor : public ClientMessageProcessor {
 public:
  auto ProcessMessage(size_t, assetmarket::Message) -> void override {
    n_proc_ += 1;
  }
  int n_proc_ = 0;
};

class AnAsioServer : public Test {
 public:
  AnAsioServer() : serv_(12345) { serv_.Start(); }
  auto AddConnections(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
      serv_.AddConnection(TEST_CLIENTS[i]);
    }
  }
  auto AddSubjects(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
      serv_.AddSubject(i, {TEST_CLIENTS[i]});
    }
  }
  std::vector<std::shared_ptr<MockConnection>> TEST_CLIENTS{
      MakeMockConnection(), MakeMockConnection(), MakeMockConnection(),
      MakeMockConnection(), MakeMockConnection()};
  AsioServer serv_;
};

TEST_F(AnAsioServer, CanSendMessagesToAddedSubjects) {
  auto conn = MakeMockConnection();
  auto conn2 = MakeMockConnection();
  ConnectionInfo info{0, "", conn};
  ConnectionInfo info2{0, "", conn2};
  serv_.AddSubject(2, info);
  serv_.AddSubject(1, info2);
  serv_.Send(2, TEST_MESSAGE);
  ASSERT_THAT(conn->n_mess_, Eq(1));
  ASSERT_THAT(conn2->n_mess_, Eq(0));
}

TEST_F(AnAsioServer, ThrowsIfSendingToInvalidSubject) {
  ASSERT_ANY_THROW(serv_.Send(2, TEST_MESSAGE));
  serv_.AddSubject(2, ConnectionInfo());
  ASSERT_ANY_THROW(serv_.Send(0, TEST_MESSAGE));
}

TEST_F(AnAsioServer, HasNoSubjectsOnStart) {
  ASSERT_THAT(serv_.SubjectCount(), Eq(0));
}

TEST_F(AnAsioServer, SubjectsCountedTowardsConnections) {
  AddSubjects(3);
  ASSERT_THAT(serv_.SubjectCount(), Eq(3));
}

TEST_F(AnAsioServer, SendsMessagesToAllSubjects) {
  AddSubjects(3);
  serv_.SendAll(TEST_MESSAGE);
  for (unsigned int i = 0; i < 3; ++i) {
    ASSERT_THAT(TEST_CLIENTS[i]->n_mess_, Eq(1));
  }
}

TEST_F(AnAsioServer, DoesNotSendToUnknownConnections) {
  AddSubjects(3);
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
  serv_.StopAcceptingClients();
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
