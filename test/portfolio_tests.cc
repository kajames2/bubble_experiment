#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "portfolio.hh"

using namespace ::testing;

class APortfolio : public Test {
 public:
  Portfolio portfolio_;
  inline static const Item A{Item::Cash};
  inline static const Item B{Item::Shares};
};

TEST_F(APortfolio, IsEmptyOnCreation) { ASSERT_TRUE(portfolio_.IsEmpty()); }

TEST_F(APortfolio, IsNotEmptyOnAdd) { portfolio_.Add(A, 5); }

TEST_F(APortfolio, ReturnsCountAfterAdd) {
  portfolio_.Add(A, 5);
  ASSERT_THAT(portfolio_.ItemCount(A), Eq(5));
}

TEST_F(APortfolio, ReturnsSumAfterTwoAdds) {
  portfolio_.Add(A, 8);
  portfolio_.Add(A, 7);
  ASSERT_THAT(portfolio_.ItemCount(A), Eq(8 + 7));
}

TEST_F(APortfolio, ReturnsItemForCorrectItem) {
  portfolio_.Add(A, 5);
  portfolio_.Add(B, 6);
  ASSERT_THAT(portfolio_.ItemCount(A), Eq(5));
}

TEST_F(APortfolio, ThrowsIfAddsZero) {
  ASSERT_THROW(portfolio_.Add(A, 0), InvalidChangeByZero);
}

TEST_F(APortfolio, ReducesItemCountOnSubtract) {
  portfolio_.Add(A, 8);
  portfolio_.Subtract(A, 9);
  ASSERT_THAT(portfolio_.ItemCount(A), Eq(8 - 9));
}

TEST_F(APortfolio, ThrowsIfSubtractsZero) {
  ASSERT_THROW(portfolio_.Subtract(A, 0), InvalidChangeByZero);
}

TEST_F(APortfolio, IsEqualWhenSameKeyValues) {
  Portfolio other = Portfolio();
  other.Add(A, 8);
  other.Add(B, 2);
  portfolio_.Add(B, 2);
  portfolio_.Add(A, 8);
  ASSERT_THAT(portfolio_, Eq(other));
}

TEST_F(APortfolio, IsUnequalWithDifferentValues) {
  Portfolio other = Portfolio();
  other.Add(A, 8);
  other.Add(B, 2);
  portfolio_.Add(B, 2);
  portfolio_.Add(A, 7);
  ASSERT_THAT(portfolio_, Ne(other));
}
