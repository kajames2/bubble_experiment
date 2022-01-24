#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "offer_processor_market.hh"
#include "offer_test_consts.hh"
#include "portfolio.hh"

using namespace ::testing;

auto InitAllocations() -> std::shared_ptr<PortfolioSet> {
  auto folio = std::make_shared<PortfolioSet>();
  for (unsigned int i = 0; i < 3; ++i) {
    folio->try_emplace(i, Portfolio());
  }
  folio->at(0).Add(Item::Cash, 600);
  folio->at(0).Add(Item::Shares, 1);
  folio->at(1).Add(Item::Cash, 400);
  folio->at(1).Add(Item::Shares, 2);
  folio->at(2).Add(Item::Cash, 200);
  folio->at(2).Add(Item::Shares, 3);
  return folio;
}

class AnOfferProcessorMarket : public Test {
 public:
  AnOfferProcessorMarket()
      : folio_(InitAllocations()), proc_(Market(), folio_) {
    InitAllocations();
  }

  unsigned int next_id_ = 0;
  auto ProcessBid(unsigned int pid, int price) -> void {
    proc_.ProcessOffer(Offer(++next_id_, pid, price, EARLY_T));
  }
  auto ProcessAsk(unsigned int pid, int price) -> void {
    proc_.ProcessOffer(Offer(++next_id_, pid, -price, EARLY_T));
  }

  std::shared_ptr<PortfolioSet> folio_;
  OfferProcessorMarket proc_;
};

TEST_F(AnOfferProcessorMarket, DoesNotModifyPortfolioOnNonTrade) {
  ProcessBid(0, 200);
  ProcessAsk(2, 300);
  PortfolioSet copy(*folio_);
  ASSERT_THAT(*folio_, Eq(copy));
}

TEST_F(AnOfferProcessorMarket, TransfersOneShareOnTrade) {
  ProcessBid(0, 200);
  ProcessAsk(2, 200);
  ASSERT_THAT(folio_->at(0).ItemCount(Item::Shares), Eq(2));
  ASSERT_THAT(folio_->at(2).ItemCount(Item::Shares), Eq(2));
}

TEST_F(AnOfferProcessorMarket, TransfersCashPriceOnTrade) {
  ProcessBid(0, 200);
  ProcessAsk(2, 200);
  ASSERT_THAT(folio_->at(0).ItemCount(Item::Cash), Eq(400));
  ASSERT_THAT(folio_->at(2).ItemCount(Item::Cash), Eq(400));
}
