#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "market_submission_result_message.hh"

using namespace ::testing;
using namespace assetmarket;

class AMarketSubmissionResultMessage : public Test {
 public:
  AMarketSubmissionResultMessage() {}
};

TEST_F(AMarketSubmissionResultMessage, Serialize) {
  Offer off1 = {0, 0, LOW_P, LATE_T};
  Offer off2 = {2, 5, LOW_P, LATE_T};
  ASSERT_THAT(off1, Eq(off2));
}
