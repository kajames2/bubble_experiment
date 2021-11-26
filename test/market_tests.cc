#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "clock.hh"
#include "market.hh"

using namespace ::testing;

class MockClock : public Clock {
 public:
  void SetTime(unsigned int time) { time_ = time; }
  unsigned int Time() const override { return time_; }

 private:
  unsigned int time_;
};

class AMarket : public Test {
 public:
  AMarket() {
    clock_ = std::make_shared<MockClock>();
    market_.SetClock(clock_);
  }
  std::shared_ptr<MockClock> clock_;
  Market market_;
  static constexpr int LOW_P = 50;
  static constexpr int HIGH_P = 90;
  void AddBid(unsigned int price) { market_.AddBid(1, price); }
  void AddAsk(unsigned int price) { market_.AddAsk(1, price); }
};

TEST_F(AMarket, IsEmptyOnCreation) { ASSERT_TRUE(market_.IsEmpty()); }

TEST_F(AMarket, IsNotEmptyAfterAddedBid) {
  AddBid(LOW_P);
  ASSERT_FALSE(market_.IsEmpty());
}

TEST_F(AMarket, BidsReturnedFromHighestToLowest) {
  AddBid(LOW_P);
  AddBid(HIGH_P);
  auto bids = market_.GetBids();
  std::vector<unsigned int> prices;
  for (const auto& b : bids) {
    prices.push_back(b.price);
  }
  ASSERT_THAT(prices, ElementsAre(HIGH_P, LOW_P));
}

TEST_F(AMarket, StandingBidIsHighest) {
  AddBid(LOW_P);
  AddBid(HIGH_P);
  auto bid = market_.GetStandingBid();
  ASSERT_THAT(bid.price, Eq(HIGH_P));
}

TEST_F(AMarket, IsEmptyAfterClear) {
  AddBid(LOW_P);
  AddAsk(HIGH_P);
  market_.clear();
  ASSERT_TRUE(market_.IsEmpty());
}

TEST_F(AMarket, IsNotEmptyAfterAddedAsk) {
  AddAsk(LOW_P);
  ASSERT_FALSE(market_.IsEmpty());
}

TEST_F(AMarket, AsksReturnedFromLowestToHighest) {
  AddAsk(LOW_P);
  AddAsk(HIGH_P);
  auto asks = market_.GetAsks();
  std::vector<unsigned int> prices;
  for (const auto& a : asks) {
    prices.push_back(a.price);
  }
  ASSERT_THAT(prices, ElementsAre(LOW_P, HIGH_P));
}

TEST_F(AMarket, StandingAskIsLowest) {
  AddAsk(LOW_P);
  AddAsk(HIGH_P);
  auto ask = market_.GetStandingAsk();
  ASSERT_THAT(ask.price, Eq(LOW_P));
}

TEST_F(AMarket, MarketAssignsIDsWhenAdded) {
  AddAsk(LOW_P);
  AddAsk(HIGH_P);
  AddBid(LOW_P);
  AddBid(HIGH_P);
  auto bids = market_.GetBids();
  auto asks = market_.GetAsks();
  std::set<unsigned int> ids;
  for (const auto& b : bids) {
    ids.insert(b.id);
  }
  for (const auto& a : asks) {
    ids.insert(a.id);
  }
  ASSERT_THAT(ids.size(), Eq(4));
}

TEST_F(AMarket, MarketTimeStampsWhenAdded) {
  clock_->SetTime(100);
  AddBid(HIGH_P);
  clock_->SetTime(150);
  AddAsk(LOW_P);
  ASSERT_THAT(market_.GetStandingBid().timestamp, Eq(100));
  ASSERT_THAT(market_.GetStandingAsk().timestamp, Eq(150));
}
