#include "market.hh"

#include <iostream>
#include <optional>

auto CanTrade(const OfferQueue& bids, const OfferQueue& asks) -> bool;

auto Market::IsEmpty() const -> bool { return bids_.empty() && asks_.empty(); }

auto Market::Bids() const -> OfferQueue { return bids_; }

auto Market::StandingBid() const -> std::optional<Offer> {
  return bids_.StandingOffer();
}

auto Market::Asks() const -> OfferQueue { return asks_; }

auto Market::StandingAsk() const -> std::optional<Offer> {
  return asks_.StandingOffer();
}

auto Market::Retract(unsigned int id) -> void {
  bids_.Retract(id);
  asks_.Retract(id);
}

auto Market::clear() -> void {
  bids_.clear();
  asks_.clear();
}

auto Market::PeekPotentialTrade() const -> std::optional<Trade> {
  if (!CanTrade(bids_, asks_)) return std::nullopt;
  Offer bid = *StandingBid();
  Offer ask = *StandingAsk();
  unsigned int timestamp = std::max(bid.timestamp, ask.timestamp);
  unsigned int price = 0;
  if (bid.timestamp > ask.timestamp) {
    price = static_cast<unsigned int>(-ask.price);
  } else {
    price = static_cast<unsigned int>(bid.price);
  }
  Trade trade = {bid.player_id, ask.player_id, price,
                 timestamp,     bid.id,        ask.id};
  return {trade};
}

auto Market::PopTrade() -> std::optional<Trade> {
  auto trade = PeekPotentialTrade();
  if (trade) {
    bids_.Retract(trade->bid_id);
    asks_.Retract(trade->ask_id);
  }
  return trade;
}

auto Market::AddOffer(Offer offer) -> void {
  if (offer.price < 0) {
    asks_.push_back(offer);
  } else {
    bids_.push_back(offer);
  }
}

auto Market::ProcessOffer(Offer offer) -> std::optional<Trade> {
  AddOffer(offer);
  return PopTrade();
}

auto CanTrade(const OfferQueue& bids, const OfferQueue& asks) -> bool {
  if (bids.empty() || asks.empty()) return false;
  return (bids.StandingOffer()->price + asks.StandingOffer()->price >= 0);
}
