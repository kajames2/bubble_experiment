#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "clock.hh"
#include "market.hh"
#include "offer_test_consts.hh"

using namespace ::testing;

class AMarket : public Test {
 public:
  AMarket() {}
  Market market_;
  unsigned int id = 0;
  void AddBid(int price, unsigned int time = EARLY_T) {
    market_.AddOffer({++id, 1, price, time});
  }
  void AddAsk(int price, unsigned int time = EARLY_T) {
    market_.AddOffer({++id, 1, -price, time});
  }
  std::optional<Trade> ProcessBid(int price, unsigned int time = EARLY_T) {
    return market_.ProcessOffer({++id, 1, price, time});
  }
  std::optional<Trade> ProcessAsk(int price, unsigned int time = EARLY_T) {
    return market_.ProcessOffer({++id, 1, -price, time});
  }
};

TEST_F(AMarket, IsEmptyOnCreation) { ASSERT_TRUE(market_.IsEmpty()); }

TEST_F(AMarket, IsNotEmptyAfterAddedBid) {
  AddBid(LOW_P);
  ASSERT_FALSE(market_.IsEmpty());
}

TEST_F(AMarket, StandingBidIsEmptyWhenNoBids) {
  auto bid = market_.StandingBid();
  ASSERT_FALSE(bid);
}

TEST_F(AMarket, StandingBidIsHighest) {
  AddBid(LOW_P);
  AddBid(HIGH_P);
  auto bid = market_.StandingBid();
  ASSERT_THAT(bid->price, Eq(HIGH_P));
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

TEST_F(AMarket, BidsArePositivePrice) {
  AddBid(LOW_P);
  ASSERT_THAT(market_.StandingBid()->price, Eq(LOW_P));
}

TEST_F(AMarket, AsksAreNegativePrice) {
  AddAsk(LOW_P);
  ASSERT_THAT(market_.StandingAsk()->price, Eq(-LOW_P));
}

TEST_F(AMarket, StandingAskIsEmptyWhenNoAsks) {
  auto ask = market_.StandingAsk();
  ASSERT_FALSE(ask);
}

TEST_F(AMarket, StandingAskIsLowest) {
  AddAsk(LOW_P);
  AddAsk(HIGH_P);
  auto ask = market_.StandingAsk();
  ASSERT_THAT(ask->price, Eq(-LOW_P));
}

TEST_F(AMarket, RetractDoesNothingWhenNoMatch) {
  AddBid(HIGH_P);
  AddAsk(LOW_P);
  auto id_bid = market_.StandingBid()->id;
  auto id_ask = market_.StandingAsk()->id;
  market_.Retract(id_bid + id_ask + 1);
  ASSERT_TRUE(market_.StandingBid());
  ASSERT_TRUE(market_.StandingAsk());
}

TEST_F(AMarket, RetractRemovesBidWithID) {
  AddBid(HIGH_P);
  AddBid(LOW_P);
  auto id_bid = market_.StandingBid()->id;
  market_.Retract(id_bid);
  ASSERT_THAT(market_.StandingBid()->price, Eq(LOW_P));
  ASSERT_THAT(market_.Bids().size(), Eq(1));
}

TEST_F(AMarket, RetractRemovesAskWithID) {
  AddAsk(HIGH_P);
  AddAsk(LOW_P);
  auto id_ask = market_.StandingAsk()->id;
  market_.Retract(id_ask);
  ASSERT_THAT(market_.StandingAsk()->price, Eq(-HIGH_P));
  ASSERT_THAT(market_.Asks().size(), Eq(1));
}

TEST_F(AMarket, ReturnsNullWhenNoPossibleTrade) {
  AddBid(LOW_P);
  AddAsk(HIGH_P);
  auto trade = market_.PeekPotentialTrade();
  ASSERT_FALSE(trade);
}

TEST_F(AMarket, ReturnsATradeWhenExists) {
  AddAsk(HIGH_P);
  AddAsk(LOW_P);
  AddBid(HIGH_P);
  auto trade = market_.PeekPotentialTrade();
  auto id_bid = market_.StandingBid()->id;
  auto id_ask = market_.StandingAsk()->id;
  ASSERT_TRUE(trade);
  ASSERT_THAT(trade->bid_id, Eq(id_bid));
  ASSERT_THAT(trade->ask_id, Eq(id_ask));
}

TEST_F(AMarket, TradeHasLaterOfferTimestamp) {
  AddBid(HIGH_P, EARLY_T);
  AddAsk(LOW_P, LATE_T);
  auto trade = market_.PeekPotentialTrade();
  ASSERT_TRUE(trade);
  ASSERT_THAT(trade->timestamp, Eq(LATE_T));
}

TEST_F(AMarket, TradeHasEarlierOfferPrice) {
  AddBid(HIGH_P, EARLY_T);
  AddAsk(LOW_P, LATE_T);
  auto trade = market_.PeekPotentialTrade();
  ASSERT_TRUE(trade);
  ASSERT_THAT(trade->price, Eq(HIGH_P));
}

TEST_F(AMarket, TradingPopsOffers) {
  AddBid(HIGH_P);
  AddAsk(LOW_P);
  auto trade = market_.PopTrade();
  ASSERT_TRUE(trade);
  ASSERT_TRUE(market_.IsEmpty());
}

TEST_F(AMarket, ReturnsNoneNonTradingOffer) {
  auto trade = ProcessBid(LOW_P);
  ASSERT_FALSE(trade);
  trade = ProcessAsk(HIGH_P);
  ASSERT_FALSE(trade);
}

TEST_F(AMarket, ReturnsTradeOnTradingOffer) {
  auto trade = ProcessBid(HIGH_P);
  ASSERT_FALSE(trade);
  trade = ProcessAsk(LOW_P);
  ASSERT_TRUE(trade);
}

TEST_F(AMarket, RemovesOffersAfterTrades) {
  auto trade = ProcessBid(MED_P);
  trade = ProcessBid(HIGH_P);
  trade = ProcessAsk(LOW_P);
  trade = ProcessAsk(LOW_P);
  ASSERT_TRUE(trade);
  trade = ProcessAsk(LOW_P);
  ASSERT_FALSE(trade);
}
