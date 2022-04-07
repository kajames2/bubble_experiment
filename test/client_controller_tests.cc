#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "client_controller.hh"
#include "message.hh"
#include "offer.hh"
#include "offer_processor.hh"
#include "offer_test_consts.hh"
#include "portfolio.hh"
#include "server.hh"
#include <vector>
#include <set>

using namespace ::testing;
using namespace assetmarket;

class SimpleMessageReceiver {
 public:
  auto Receive(const assetmarket::Message&) { n_messages_ += 1; }
  int n_messages_ = 0;
};

class MockServer : public Server {
 public:
  MockServer(int n_players) : clients_() {
    for (int i = 0; i < n_players; ++i)
      clients_.push_back(SimpleMessageReceiver());
  }
  virtual auto Send(size_t id, const assetmarket::Message& message)
      -> void override {
    clients_[id].Receive(message);
  }
  virtual auto SendAll(const assetmarket::Message& message) -> void override {
    for (auto& client : clients_) {
      client.Receive(message);
    }
  }
  std::vector<SimpleMessageReceiver> clients_;
};

class MockOfferProcessor : public OfferProcessor {
 public:
  auto ProcessOffer(Offer offer) -> MarketSubmissionResult override {
    return {MarketSubmissionStatus::Accepted, offer, std::nullopt};
  }
};

class MockClock : public Clock {
 public:
  void SetTime(unsigned int time) { time_ = time; }
  Time GetTime() const override { return time_; }

 private:
  unsigned int time_;
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
  ASSERT_THAT(res.offer->timestamp, Eq(EARLY_T));
}

TEST_F(AClientController, TakeAskSubmittedOfferHasNegativePrice) {
  auto res = cont_.TakeAsk(VALID_PID, LOW_P);
  ASSERT_THAT(res.offer->price, Eq(-LOW_P));
}

TEST_F(AClientController, TakeAskTimestampsOffer) {
  clock_->SetTime(EARLY_T);
  auto res = AddAsk(VALID_PID);
  ASSERT_THAT(res.offer->timestamp, Eq(EARLY_T));
}

TEST_F(AClientController, ControllerAssignsDifferentIDsWhenAdded) {
  std::set<unsigned int> ids;
  for (int i = 0; i < 3; ++i) {
    auto res = AddBid(VALID_PID);
    ids.insert(res.offer->id);

    res = AddAsk(VALID_PID);
    ids.insert(res.offer->id);
  }
  ASSERT_THAT(ids.size(), Eq(6));
}

TEST_F(AClientController, ControllerTellsClientsOnSuccessfulOffer) {
  cont_.TakeAsk(VALID_PID, LOW_P);
  for (const auto& client : serv_->clients_) {
    ASSERT_THAT(client.n_messages_, Eq(1));
  }
}
