#ifndef OFFER_PROCESSOR_MARKET_HH
#define OFFER_PROCESSOR_MARKET_HH

#include "market.hh"
#include "offer_processor.hh"
#include "portfolio.hh"
#include <vector>

namespace assetmarket {
using PortfolioSet = std::vector<Portfolio>;

class OfferProcessorMarket : public OfferProcessor {
 public:
  OfferProcessorMarket(Market market, std::shared_ptr<PortfolioSet>);
  virtual auto ProcessOffer(Offer offer) -> MarketSubmissionResult override;

 private:
  Market market_;
  std::shared_ptr<PortfolioSet> folio_;
  auto ProcessTrade(const std::optional<Trade>& trade) -> void;
};
}  // namespace assetmarket
#endif  // OFFER_PROCESSOR_MARKET_HH
