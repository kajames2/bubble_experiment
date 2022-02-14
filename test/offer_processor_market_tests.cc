#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "offer_processor_market.hh"
#include "offer_test_consts.hh"
#include "portfolio.hh"

using namespace ::testing;
using namespace assetmarket;

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
  auto ProcessBid(unsigned int pid, int price) -> MarketSubmissionResult {
    return proc_.ProcessOffer(Offer{++next_id_, pid, price, EARLY_T});
  }
  auto ProcessAsk(unsigned int pid, int price) -> MarketSubmissionResult {
    return proc_.ProcessOffer(Offer{++next_id_, pid, -price, EARLY_T});
  }

  std::shared_ptr<PortfolioSet> folio_;
  OfferProcessorMarket proc_;
};

TEST_F(AnOfferProcessorMarket, DoesNotModifyPortfolioOnNonTrade) {
  ProcessBid(0, LOW_P);
  ProcessAsk(2, HIGH_P);
  PortfolioSet copy(*folio_);
  ASSERT_THAT(*folio_, Eq(copy));
}

TEST_F(AnOfferProcessorMarket, TransfersOneShareOnTrade) {
  auto sh0 = folio_->at(0).ItemCount(Item::Shares);
  auto sh2 = folio_->at(2).ItemCount(Item::Shares);
  ProcessBid(0, HIGH_P);
  ProcessAsk(2, HIGH_P);
  ASSERT_THAT(folio_->at(0).ItemCount(Item::Shares), Eq(sh0 + 1));
  ASSERT_THAT(folio_->at(2).ItemCount(Item::Shares), Eq(sh2 - 1));
}

TEST_F(AnOfferProcessorMarket, TransfersCashPriceOnTrade) {
  auto cash0 = folio_->at(0).ItemCount(Item::Cash);
  auto cash2 = folio_->at(2).ItemCount(Item::Cash);
  ProcessBid(0, HIGH_P);
  ProcessAsk(2, HIGH_P);
  ASSERT_THAT(folio_->at(0).ItemCount(Item::Cash), Eq(cash0 - HIGH_P));
  ASSERT_THAT(folio_->at(2).ItemCount(Item::Cash), Eq(cash2 + HIGH_P));
}

TEST_F(AnOfferProcessorMarket, ReturnsOfferAndStatusOnAcceptedOffer) {
  auto res = ProcessBid(2, HIGH_P);
  ASSERT_TRUE(res.offer);
  ASSERT_THAT(res.status, Eq(MarketSubmissionStatus::Accepted));
}

TEST_F(AnOfferProcessorMarket, ReturnsTradeOnlyOnTrade) {
  auto res1 = ProcessBid(2, LOW_P);
  auto res2 = ProcessAsk(1, HIGH_P);
  auto res3 = ProcessAsk(0, LOW_P);
  ASSERT_FALSE(res1.trade);
  ASSERT_FALSE(res2.trade);
  ASSERT_TRUE(res3.trade);
}
