#include "offer_processor_market.hh"

OfferProcessorMarket::OfferProcessorMarket(Market market,
                                           std::shared_ptr<PortfolioSet> port)
    : market_(std::move(market)), port_(port) {}

auto OfferProcessorMarket::ProcessOffer(Offer offer) -> void {
  auto trade = market_.ProcessOffer(offer);
  ProcessTrade(trade);
}

auto OfferProcessorMarket::ProcessTrade(const std::optional<Trade>& trade)
    -> void {
  if (trade) {
    port_->at(trade->buyer).Add(Item::Shares, 1);
    port_->at(trade->seller).Subtract(Item::Shares, 1);
    port_->at(trade->buyer).Subtract(Item::Cash, trade->price);
    port_->at(trade->seller).Add(Item::Cash, trade->price);
  }
}
