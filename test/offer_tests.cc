#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "offer.hh"
#include "offer_test_consts.hh"

using namespace ::testing;
using namespace assetmarket;

class AnOffer : public Test {
 public:
  AnOffer() {}
};

TEST_F(AnOffer, EqualityIgnoresIDs) {
  Offer off1 = {0, 0, LOW_P, LATE_T};
  Offer off2 = {2, 5, LOW_P, LATE_T};
  ASSERT_THAT(off1, Eq(off2));
}

TEST_F(AnOffer, ComparesPriceFirst) {
  Offer off1 = {0, 0, LOW_P, EARLY_T};
  Offer off2 = {2, 5, HIGH_P, LATE_T};
  ASSERT_THAT(off1, Lt(off2));
}

TEST_F(AnOffer, ComparesTimeReverse) {
  Offer off1 = {0, 0, HIGH_P, LATE_T};
  Offer off2 = {2, 5, HIGH_P, EARLY_T};
  ASSERT_THAT(off1, Lt(off2));
}
