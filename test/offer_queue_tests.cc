#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "offer_queue.hh"
#include "offer_test_consts.hh"

using namespace ::testing;

class AnOfferQueue : public Test {
 public:
  OfferQueue queue_;
  void Add(unsigned int id, unsigned int player_id, int price) {
    queue_.push_back(Offer{id, player_id, price, 0});
  }
  void AddTwo() {
    Add(0, 0, HIGH_P);
    Add(1, 0, LOW_P);
  }
};

TEST_F(AnOfferQueue, IsEmptyOnCreation) { ASSERT_TRUE(queue_.empty()); }

TEST_F(AnOfferQueue, IsNotEmptyAfterAddedBid) {
  Add(0, 0, LOW_P);
  ASSERT_FALSE(queue_.empty());
}

TEST_F(AnOfferQueue, SizeZeroOnCreation) { ASSERT_THAT(queue_.size(), Eq(0)); }

TEST_F(AnOfferQueue, SizeEqualsNumberAdded) {
  AddTwo();
  ASSERT_THAT(queue_.size(), Eq(2));
}

TEST_F(AnOfferQueue, StandingBidIsEmptyWhenNoBids) {
  auto offer = queue_.StandingOffer();
  ASSERT_FALSE(offer);
}

TEST_F(AnOfferQueue, StandingOfferIsGreatest) {
  AddTwo();
  auto offer = queue_.StandingOffer();
  ASSERT_THAT(offer->price, Eq(HIGH_P));
}

TEST_F(AnOfferQueue, IsEmptyAfterClear) {
  AddTwo();
  queue_.clear();
  ASSERT_TRUE(queue_.empty());
}

TEST_F(AnOfferQueue, RetractDoesNothingWhenNoMatch) {
  AddTwo();
  queue_.Retract(2);
  ASSERT_TRUE(queue_.StandingOffer());
  ASSERT_THAT(queue_.size(), Eq(2));
}

TEST_F(AnOfferQueue, RetractRemovesBidWithID) {
  AddTwo();
  queue_.Retract(0);
  ASSERT_THAT(queue_.StandingOffer()->price, Eq(LOW_P));
  ASSERT_THAT(queue_.size(), Eq(1));
}
