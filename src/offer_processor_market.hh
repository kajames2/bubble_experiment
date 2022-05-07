#ifndef OFFER_PROCESSOR_MARKET_HH
#define OFFER_PROCESSOR_MARKET_HH

#include "market.hh"
#include "offer_processor.hh"
#include "portfolio.hh"
#include "subject.hh"

namespace assetmarket {
using PortfolioSet = std::unordered_map<SubjectID, Portfolio>;

class OfferProcessorMarket : public OfferProcessor {
 public:
  OfferProcessorMarket(Market market, std::shared_ptr<PortfolioSet>);
  virtual auto ProcessOffer(Offer offer) -> MarketSubmissionResult override;
  virtual auto ProcessCreate(SubjectID id) -> CreationResult override;
  virtual auto ProcessRetract(RetractRequest rr) -> RetractResult override;

 private:
  Market market_;
  std::shared_ptr<PortfolioSet> folio_;
  auto ProcessTrade(const std::optional<Trade>& trade) -> void;
};
}  // namespace assetmarket
#endif  // OFFER_PROCESSOR_MARKET_HH
