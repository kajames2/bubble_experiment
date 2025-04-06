#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cmath>

#include "communication_test_consts.hh"
#include "experiment_controller.hh"
#include "mock_experiment.hh"
#include "mock_server.hh"

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

class AnExperimentController : public Test {
 public:
  AnExperimentController()
      : serv_(std::make_shared<MockServer>(10)),
        exp(std::make_shared<MockExperiment>()),
        cont_(serv_, exp) {}

  std::shared_ptr<MockServer> serv_;
  std::shared_ptr<MockExperiment> exp;
  ExperimentController cont_;
};

TEST_F(AnExperimentController, CanRequestToStartExperiment) {
  cont_.StartExperiment();
  ASSERT_THAT(exp->n_start_exp, Eq(1));
}

TEST_F(AnExperimentController, NotifiesSubjectsOnExperimentStart) {
  cont_.StartExperiment();
  for (const auto& [key, count] : serv_->counts_) {
    ASSERT_THAT(count, Eq(1));
  }
}
