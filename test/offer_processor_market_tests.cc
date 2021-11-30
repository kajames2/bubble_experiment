#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "offer_processor_market.hh"
#include "offer_test_consts.hh"
#include "portfolio.hh"

using namespace ::testing;

auto InitAllocations() -> std::shared_ptr<PortfolioSet> {
  auto port = std::make_shared<PortfolioSet>();
  for (unsigned int i = 0; i < 3; ++i) {
    port->try_emplace(i, Portfolio());
  }
  port->at(0).Add(Item::Cash, 600);
  port->at(0).Add(Item::Shares, 1);
  port->at(1).Add(Item::Cash, 400);
  port->at(1).Add(Item::Shares, 2);
  port->at(2).Add(Item::Cash, 200);
  port->at(2).Add(Item::Shares, 3);
  return port;
}

class AnOfferProcessorMarket : public Test {
 public:
  AnOfferProcessorMarket() : port_(InitAllocations()), proc_(Market(), port_) {
    InitAllocations();
  }

  unsigned int id = 0;
  void ProcessBid(unsigned int pid, int price) {
    proc_.ProcessOffer(Offer(++id, pid, price, EARLY_T));
  }
  void ProcessAsk(unsigned int pid, int price) {
    proc_.ProcessOffer(Offer(++id, pid, -price, EARLY_T));
  }

  std::shared_ptr<PortfolioSet> port_;
  OfferProcessorMarket proc_;
};

TEST_F(AnOfferProcessorMarket, DoesNotModifyPortfolioOnNonTrade) {
  ProcessBid(0, 200);
  ProcessAsk(2, 300);
  PortfolioSet copy(*port_);
  ASSERT_THAT(*port_, Eq(copy));
}

TEST_F(AnOfferProcessorMarket, TransfersOneShareOnTrade) {
  ProcessBid(0, 200);
  ProcessAsk(2, 200);
  ASSERT_THAT(port_->at(0).ItemCount(Item::Shares), Eq(2));
  ASSERT_THAT(port_->at(2).ItemCount(Item::Shares), Eq(2));
}

TEST_F(AnOfferProcessorMarket, TransfersCashPriceOnTrade) {
  ProcessBid(0, 200);
  ProcessAsk(2, 200);
  ASSERT_THAT(port_->at(0).ItemCount(Item::Cash), Eq(400));
  ASSERT_THAT(port_->at(2).ItemCount(Item::Cash), Eq(400));
}
