#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "configuration_consts.hh"
#include "experiment.hh"
#include "mock_server.hh"

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

class AnExperiment : public Test {
 public:
  AnExperiment() {}
  Experiment exp;
};

TEST_F(AnExperiment, HasNoSubjectsOnInitialization) {
  ASSERT_THAT(exp.NSubjects(), Eq(0));
}

TEST_F(AnExperiment, CanAddSubject) {
  exp.AddSubject(Subject());
  exp.AddSubject(Subject());
  ASSERT_THAT(exp.NSubjects(), Eq(2));
}

TEST_F(AnExperiment, InitializesToNotStarted) {
  ASSERT_THAT(exp.GetStage(), Eq(Stage::NotStarted));
}

TEST_F(AnExperiment, NewPeriodIgnoredIfNotStarted) {
  auto cur_period = exp.GetPeriod();
  exp.NewPeriod();
  ASSERT_THAT(exp.GetStage(), Eq(Stage::NotStarted));
  ASSERT_THAT(exp.GetPeriod(), Eq(cur_period));
}

TEST_F(AnExperiment, StageIsStartedOnStart) {
  exp.Start();
  ASSERT_THAT(exp.GetStage(), Eq(Stage::Started));
}

TEST_F(AnExperiment, IncrementsPeriodWhenNewPeriod) {
  exp.Start();
  auto cur_period = exp.GetPeriod();
  exp.NewPeriod();
  ASSERT_THAT(exp.GetPeriod(), Eq(cur_period + 1));
}
