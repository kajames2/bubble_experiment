#include "offer_processor_market.hh"

namespace assetmarket {
OfferProcessorMarket::OfferProcessorMarket(Market market,
                                           std::shared_ptr<PortfolioSet> port)
    : market_(std::move(market)), folio_(port) {}

auto OfferProcessorMarket::ProcessOffer(Offer offer) -> MarketSubmissionResult {
  auto trade = market_.ProcessOffer(offer);
  ProcessTrade(trade);
  return {offer, trade};
}

auto OfferProcessorMarket::ProcessTrade(const std::optional<Trade>& trade)
    -> void {
  if (trade) {
    (*folio_)[trade->buyer].Add(Item::Shares, 1);
    (*folio_)[trade->seller].Subtract(Item::Shares, 1);
    (*folio_)[trade->buyer].Subtract(Item::Cash, trade->price);
    (*folio_)[trade->seller].Add(Item::Cash, trade->price);
  }
}
}  // namespace assetmarket
