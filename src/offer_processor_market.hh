#ifndef OFFER_PROCESSOR_MARKET_HH
#define OFFER_PROCESSOR_MARKET_HH

#include "market.hh"
#include "offer_processor.hh"
#include "portfolio.hh"

using PortfolioSet = std::unordered_map<unsigned int, Portfolio>;

class OfferProcessorMarket : public OfferProcessor {
 public:
  OfferProcessorMarket(Market market, std::shared_ptr<PortfolioSet>);

  virtual auto ProcessOffer(Offer offer) -> void override;

 private:
  Market market_;
  std::shared_ptr<PortfolioSet> port_;
  auto ProcessTrade(const std::optional<Trade>& trade) -> void;
};

#endif  // OFFER_PROCESSOR_MARKET_HH
