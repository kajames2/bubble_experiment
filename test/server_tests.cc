#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "connection.hh"
#include "server.hh"

using namespace ::testing;

// class MockConnection : public Connection {};

class AServer : public Test {
 public:
  AServer() {}
  Server server;
};

// TEST_F(AServer, ) {
// }
