#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "client_controller.hh"
#include "offer.hh"
#include "offer_processor.hh"
#include "portfolio.hh"

using namespace ::testing;

class MockOfferProcessor : public OfferProcessor {
 public:
  auto ProcessOffer(Offer offer) -> MarketSubmissionStatus override {
    if (offer.player_id == 0)
      return MarketSubmissionStatus::InvalidPlayer;
    else
      return MarketSubmissionStatus::Accepted;
  }
};

class AClientController : public Test {
 public:
  AClientController()
      : proc_(std::make_unique<MockOfferProcessor>()),
        cont_(std::move(proc_)) {}

  std::unique_ptr<MockOfferProcessor> proc_;
  ClientController cont_;
};

TEST_F(AClientController, TakeBidReturnsAcceptedCodeOnValidBid) {
  MarketSubmissionResult res = cont_.TakeBid(2, 100);
  ASSERT_THAT(res.status, Eq(MarketSubmissionStatus::Accepted));
}

TEST_F(AClientController, TakeBidReturnsSubmittedOfferOnValidBid) {
  MarketSubmissionResult res = cont_.TakeBid(2, 100);
  ASSERT_TRUE(res.offer);
}

TEST_F(AClientController, TakeBidDoesNotReturnOfferOnInvalidBid) {
  MarketSubmissionResult res = cont_.TakeBid(0, 100);
  ASSERT_FALSE(res.offer);
}
