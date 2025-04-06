#include "offer_processor_market.hh"

#include <optional>

#include "creation_result.hh"
#include "market_submission_result.hh"
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
    return {OfferValidity::RejectedMarketClosed, offer};
  }
  if (offer.price < 0) {
    auto n_offers = market_->Asks()
                        .Offers(static_cast<unsigned int>(offer.player_id))
                        .size();
    if (folio_->at(offer.player_id).ItemCount(Item::Shares) <=
        static_cast<int>(n_offers)) {
      return {OfferValidity::RejectedInsufficientUnits, offer};
    }
  } else {
    auto bids =
        market_->Bids().Offers(static_cast<unsigned int>(offer.player_id));
    int sum = 0;
    for (auto bid : bids) {
      sum += bid.price;
    }

    if (folio_->at(offer.player_id).ItemCount(Item::Cash) < offer.price + sum) {
      return {OfferValidity::RejectedInsufficientCash, offer};
    }
  }

  if (config_->improvement_rule) {
    if (offer.price < 0) {
      auto best_ask = market_->StandingAsk();
      if (best_ask && best_ask >= offer) {
        return {OfferValidity::RejectedNonImproving, offer};
      }
    } else {
      auto best_bid = market_->StandingBid();
      if (best_bid && best_bid >= offer) {
        return {OfferValidity::RejectedNonImproving, offer};
      }
    }
  }
  if (offer.price < 0) {
    auto best_bid = market_->StandingBid();
    if (best_bid && best_bid->player_id == offer.player_id &&
        CanTrade(*best_bid, offer)) {
      return {OfferValidity::RejectedTradeWithSelf, offer};
    }
  } else {
    auto best_ask = market_->StandingAsk();
    if (best_ask && best_ask->player_id == offer.player_id &&
        CanTrade(offer, *best_ask)) {
      return {OfferValidity::RejectedTradeWithSelf, offer};
    }
  }
  if (offer.price < -999 || offer.price > 999) {
    return {OfferValidity::RejectedPriceOutOfRange, offer};
  }

  MarketSubmissionResult res = {OfferValidity::Accepted, offer};
  if (config_->replace_offer_rule) {
    if (offer.price < 0) {
      auto offers = market_->Asks().Offers(offer.player_id);
      for (const auto& off : offers) {
        market_->Retract(off.id);
        res.retracts.push_back(
            {RetractValidity::Accepted, {offer.player_id, off.id}});
      }
    } else {
      auto offers = market_->Bids().Offers(offer.player_id);
      for (const auto& off : offers) {
        market_->Retract(off.id);
        res.retracts.push_back(
            {RetractValidity::Accepted, {offer.player_id, off.id}});
      }
    }
  }
  res.trade = market_->ProcessOffer(offer);
  ProcessTrade(res.trade);
  return res;
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
  if (exp_state_->round < config_->first_create_round ||
      !config_->GetSubjectConfiguration(id).is_creator) {
    return {CreationValidity::RejectedCannotCreate, id};
  }
  unsigned int margin =
      config_->GetRoundConfiguration(exp_state_->round).margin_req;
  if (folio_->at(id).ItemCount(Item::Cash) < static_cast<int>(margin)) {
    return {CreationValidity::RejectedInsufficientCash, id};
  }
  folio_->at(id).Add(Item::Derivatives, 1);
  folio_->at(id).Add(Item::Shares, 1);
  folio_->at(id).Add(Item::Margin, margin);
  folio_->at(id).Subtract(Item::Cash, margin);
  return {CreationValidity::Accepted, id};
}

auto OfferProcessorMarket::ProcessRetract(RetractRequest rr) -> RetractResult {
  if (!exp_state_->IsMarketOpen()) {
    return {RetractValidity::RejectedMarketClosed, rr};
  }
  auto offer = market_->GetOffer(rr.offer_id);
  if (!offer) {
    return {RetractValidity::RejectedNonExistent, rr};
  }
  if (offer->player_id != rr.id) {
    return {RetractValidity::RejectedOwnerMismatch, rr};
  }
  market_->Retract(rr.offer_id);
  return {RetractValidity::Accepted, rr};
}

auto OfferProcessorMarket::GetPortfolio(SubjectID id) const -> Portfolio {
  return folio_->at(id);
}

}  // namespace assetmarket
