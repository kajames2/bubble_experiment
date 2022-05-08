#include "offer_processor_market.hh"

#include <optional>

#include "creation_result.hh"
#include "retract_request.hh"

namespace assetmarket {
OfferProcessorMarket::OfferProcessorMarket(
    std::shared_ptr<Market> market, std::shared_ptr<PortfolioSet> port,
    std::shared_ptr<ExperimentState> exp_state,
    std::shared_ptr<Configuration> conf)
    : market_(std::move(market)),
      folio_(port),
      exp_state_(exp_state),
      config_(conf) {}

auto OfferProcessorMarket::ProcessOffer(Offer offer) -> MarketSubmissionResult {
  if (!exp_state_->IsMarketOpen()) {
    return {OfferValidity::RejectedMarketClosed, offer, std::nullopt};
  }
  if (offer.price < 0) {
    if (folio_->at(offer.player_id).ItemCount(Item::Shares) == 0) {
      return {OfferValidity::RejectedInsufficientUnits, offer, std::nullopt};
    }
  } else {
    if (folio_->at(offer.player_id).ItemCount(Item::Cash) < offer.price) {
      return {OfferValidity::RejectedInsufficientCash, offer, std::nullopt};
    }
  }

  if (config_->improvement_rule) {
    if (offer.price < 0) {
      auto best_ask = market_->StandingAsk();
      if (best_ask && best_ask >= offer) {
        return {OfferValidity::RejectedNonImproving, offer, std::nullopt};
      }
    } else {
      auto best_bid = market_->StandingBid();
      if (best_bid && best_bid >= offer) {
        return {OfferValidity::RejectedNonImproving, offer, std::nullopt};
      }
    }
  }
  if (offer.price < 0) {
    auto best_bid = market_->StandingBid();
    if (best_bid && best_bid->player_id == offer.player_id &&
        CanTrade(*best_bid, offer)) {
      return {OfferValidity::RejectedTradeWithSelf, offer, std::nullopt};
    }
  } else {
    auto best_ask = market_->StandingBid();
    if (best_ask && best_ask->player_id == offer.player_id &&
        CanTrade(offer, *best_ask)) {
      return {OfferValidity::RejectedTradeWithSelf, offer, std::nullopt};
    }
  }
  if (offer.price < -999 || offer.price > 999) {
    return {OfferValidity::RejectedPriceOutOfRange, offer, std::nullopt};
  }
  auto trade = market_->ProcessOffer(offer);
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
  if (!exp_state_->IsMarketOpen()) {
    return {CreationValidity::RejectedMarketClosed, id};
  }
  if (folio_->at(id).ItemCount(Item::Cash) < margin_) {
    return {CreationValidity::RejectedInsufficientCash, id};
  }
  if (exp_state_->round < config_->first_create_round ||
      !config_->GetSubjectConfiguration(id).is_creator) {
    return {CreationValidity::RejectedCannotCreate, id};
  }
  folio_->at(id).Add(Item::Derivatives, 1);
  folio_->at(id).Add(Item::Shares, 1);
  folio_->at(id).Add(Item::Margin, margin_);
  folio_->at(id).Subtract(Item::Cash, margin_);
  return {CreationValidity::Accepted, id};
}

auto OfferProcessorMarket::ProcessRetract(RetractRequest rr) -> RetractResult {
  if (!exp_state_->IsMarketOpen()) {
    return {RetractValidity::RejectedMarketClosed, rr};
  }
  auto offer = market_->GetOffer(rr.offer_id);
  if (!offer) {
    return {RetractValidity::RejectedNonExistant, rr};
  }
  if (offer->player_id != rr.id) {
    return {RetractValidity::RejectedOwnerMismatch, rr};
  }
  market_->Retract(rr.offer_id);
  return {RetractValidity::Accepted, rr};
}

}  // namespace assetmarket
