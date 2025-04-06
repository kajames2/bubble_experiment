#include "market.hh"

#include <algorithm>
#include <optional>

namespace exptemplate {
namespace market {
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

auto Market::GetOffer(unsigned int id) -> std::optional<Offer> {
  auto off = bids_.GetOffer(id);
  if (!off) {
    off = asks_.GetOffer(id);
  }
  return off;
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
  unsigned int time_elapsed = std::max(bid.time_elapsed, ask.time_elapsed);
  unsigned int price = 0;
  if (bid.time_elapsed > ask.time_elapsed) {
    price = static_cast<unsigned int>(-ask.price);
  } else {
    price = static_cast<unsigned int>(bid.price);
  }
  Trade trade = {bid.player_id, ask.player_id, price,
                 time_elapsed,  bid.id,        ask.id};
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

auto CanTrade(const Offer& bid, const Offer& ask) -> bool {
  return bid.price + ask.price >= 0;
}

auto CanTrade(const OfferQueue& bids, const OfferQueue& asks) -> bool {
  if (bids.empty() || asks.empty()) return false;
  return CanTrade(*bids.StandingOffer(), *asks.StandingOffer());
}
}  // namespace market
}  // namespace exptemplate
