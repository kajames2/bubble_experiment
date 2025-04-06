#ifndef OFFER_PROCESSOR_MARKET_HH
#define OFFER_PROCESSOR_MARKET_HH

#include "configuration.hh"
#include "experiment_state.hh"
#include "market.hh"
#include "offer_processor.hh"
#include "portfolio.hh"
#include "subject.hh"

namespace assetmarket {

class OfferProcessorMarket : public OfferProcessor {
 public:
  OfferProcessorMarket(std::shared_ptr<Market> market,
                       std::shared_ptr<PortfolioSet>,
                       std::shared_ptr<ExperimentState>,
                       std::shared_ptr<Configuration>);
  virtual auto ProcessOffer(Offer offer) -> MarketSubmissionResult override;
  virtual auto ProcessCreate(SubjectID id) -> CreationResult override;
  virtual auto ProcessRetract(RetractRequest rr) -> RetractResult override;
  virtual auto GetPortfolio(SubjectID id) const -> Portfolio override;

 private:
  std::shared_ptr<Market> market_;
  std::shared_ptr<PortfolioSet> folio_;
  std::shared_ptr<ExperimentState> exp_state_;
  std::shared_ptr<Configuration> config_;
  auto ProcessTrade(const std::optional<Trade>& trade) -> void;
  unsigned int margin_ = 100;
};
}  // namespace assetmarket
#endif  // OFFER_PROCESSOR_MARKET_HH
