#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cmath>

#include "communication_test_consts.hh"
#include "connection_controller.hh"
#include "mock_experiment.hh"
#include "mock_server.hh"

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

class AConnectionController : public Test {
 public:
  AConnectionController()
      : serv_(std::make_shared<MockServer>(0)),
        exp(std::make_shared<MockExperiment>()),
        cont_(serv_, exp) {}

  std::shared_ptr<MockServer> serv_;
  std::shared_ptr<MockExperiment> exp;
  ConnectionController cont_;
};

TEST_F(AConnectionController, PassesNewConnectionsToExperiment) {
  ConnectionInfo info;
  cont_.AcceptConnection(info);
  ASSERT_THAT(exp->n_subjects, Eq(1));
}

TEST_F(AConnectionController, PassesNewIDsToServer) {
  ConnectionInfo info;
  cont_.AcceptConnection(info);
  auto id = exp->used_ids.back();
  serv_->Send(id, TEST_MESSAGE);
  ASSERT_THAT(serv_->counts_.at(id), Eq(1));
}
