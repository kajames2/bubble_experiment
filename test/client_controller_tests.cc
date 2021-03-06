#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "client_controller.hh"
#include "mock_clock.hh"
#include "mock_server.hh"
#include "offer.hh"
#include "offer_processor.hh"
#include "offer_test_consts.hh"
#include <set>

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

class MockOfferProcessor : public OfferProcessor {
 public:
  auto ProcessOffer(Offer offer) -> MarketSubmissionResult override {
    return {offer, std::nullopt};
  }
};

class AClientController : public Test {
 public:
  AClientController()
      : proc_(std::make_unique<MockOfferProcessor>()),
        clock_(std::make_shared<MockClock>()),
        serv_(std::make_shared<MockServer>(10)),
        cont_(std::move(proc_), serv_, clock_) {}

  auto AddBid(unsigned int pid) -> MarketSubmissionResult {
    return cont_.TakeBid(pid, LOW_P);
  }
  auto AddAsk(unsigned int pid) -> MarketSubmissionResult {
    return cont_.TakeAsk(pid, LOW_P);
  }
  std::unique_ptr<MockOfferProcessor> proc_;
  std::shared_ptr<MockClock> clock_;
  std::shared_ptr<MockServer> serv_;
  ClientController cont_;
};

TEST_F(AClientController, TakeBidTimestampsOffer) {
  clock_->SetTime(EARLY_T);
  auto res = AddBid(VALID_PID);
  ASSERT_THAT(res.offer.time_elapsed, Eq(EARLY_T));
}

TEST_F(AClientController, TakeAskSubmittedOfferHasNegativePrice) {
  auto res = cont_.TakeAsk(VALID_PID, LOW_P);
  ASSERT_THAT(res.offer.price, Eq(-LOW_P));
}

TEST_F(AClientController, TakeAskTimestampsOffer) {
  clock_->SetTime(EARLY_T);
  auto res = AddAsk(VALID_PID);
  ASSERT_THAT(res.offer.time_elapsed, Eq(EARLY_T));
}

TEST_F(AClientController, ControllerAssignsDifferentIDsWhenAdded) {
  std::set<unsigned int> ids;
  for (int i = 0; i < 3; ++i) {
    auto res = AddBid(VALID_PID);
    ids.insert(res.offer.id);

    res = AddAsk(VALID_PID);
    ids.insert(res.offer.id);
  }
  ASSERT_THAT(ids.size(), Eq(6));
}

TEST_F(AClientController, ControllerTellsClientsOnSuccessfulOffer) {
  cont_.TakeAsk(VALID_PID, LOW_P);
  for (const auto& [key, count] : serv_->counts_) {
    ASSERT_THAT(count, Eq(1));
  }
}
