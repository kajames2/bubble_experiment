#include "offer_processor_market.hh"

#include "retract_request.hh"

namespace assetmarket {
OfferProcessorMarket::OfferProcessorMarket(Market market,
                                           std::shared_ptr<PortfolioSet> port)
    : market_(std::move(market)), folio_(port) {}

auto OfferProcessorMarket::ProcessOffer(Offer offer) -> MarketSubmissionResult {
  auto trade = market_.ProcessOffer(offer);
  ProcessTrade(trade);
  return {OfferValidity::Accepted, offer, trade};
}

auto OfferProcessorMarket::ProcessTrade(const std::optional<Trade>& trade)
    -> void {
  if (trade) {
    folio_->at(trade->buyer).Add(Item::Shares, 1);
    folio_->at(trade->seller).Subtract(Item::Shares, 1);
    folio_->at(trade->buyer).Subtract(Item::Cash, trade->price);
    folio_->at(trade->seller).Add(Item::Cash, trade->price);
  }
}

auto OfferProcessorMarket::ProcessCreate(SubjectID id) -> CreationResult {
  return {CreationValidity::Accepted, id};
}

auto OfferProcessorMarket::ProcessRetract(RetractRequest rr) -> RetractResult {
  return {RetractValidity::Accepted, rr};
}

}  // namespace assetmarket
