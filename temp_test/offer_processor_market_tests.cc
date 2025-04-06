#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "configuration_consts.hh"
#include "offer_processor_market.hh"
#include "offer_test_consts.hh"
#include "portfolio.hh"

using namespace ::testing;
using namespace assetmarket;
using namespace assettest;

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
  auto sh0 = (*folio_)[0].ItemCount(Item::Shares);
  auto sh2 = (*folio_)[2].ItemCount(Item::Shares);
  ProcessBid(0, HIGH_P);
  ProcessAsk(2, HIGH_P);
  ASSERT_THAT((*folio_)[0].ItemCount(Item::Shares), Eq(sh0 + 1));
  ASSERT_THAT((*folio_)[2].ItemCount(Item::Shares), Eq(sh2 - 1));
}

TEST_F(AnOfferProcessorMarket, TransfersCashPriceOnTrade) {
  auto cash0 = (*folio_)[0].ItemCount(Item::Cash);
  auto cash2 = (*folio_)[2].ItemCount(Item::Cash);
  ProcessBid(0, HIGH_P);
  ProcessAsk(2, HIGH_P);
  ASSERT_THAT((*folio_)[0].ItemCount(Item::Cash), Eq(cash0 - HIGH_P));
  ASSERT_THAT((*folio_)[2].ItemCount(Item::Cash), Eq(cash2 + HIGH_P));
}

TEST_F(AnOfferProcessorMarket, ReturnsTradeOnlyOnTrade) {
  auto res1 = ProcessBid(2, LOW_P);
  auto res2 = ProcessAsk(1, HIGH_P);
  auto res3 = ProcessAsk(0, LOW_P);
  ASSERT_FALSE(res1.trade);
  ASSERT_FALSE(res2.trade);
  ASSERT_TRUE(res3.trade);
}
