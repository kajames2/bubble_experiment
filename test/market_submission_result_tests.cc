#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "market_submission_result.hh"
#include "offer.hh"
#include "offer_test_consts.hh"

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

class AMarketSubmissionResult : public Test {
 public:
  AMarketSubmissionResult() {}
};

TEST_F(AMarketSubmissionResult, CanDeserializeFromSerializeWithoutTrade) {
  Offer off1 = {0, 0, LOW_P, LATE_T};
  MarketSubmissionResult res = {off1, std::nullopt};
  std::string str = to_string(res);
  MarketSubmissionResult res2;
  from_string(str, res2);
  ASSERT_THAT(res2, Eq(res));
}

TEST_F(AMarketSubmissionResult, CanDeserializeFromSerializeWithTrade) {
  Offer off1 = {0, 0, LOW_P, LATE_T};
  Trade trade = {1, 2, 30, 430, 2, 3};
  MarketSubmissionResult res = {off1, trade};
  std::string str = to_string(res);
  MarketSubmissionResult res2;
  from_string(str, res2);
  ASSERT_THAT(res2, Eq(res));
}
