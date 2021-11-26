#include "market.hh"

auto Market::IsEmpty() const -> bool { return bids_.empty() && asks_.empty(); }

auto Market::AddBid(unsigned int player_id, unsigned int price) -> void {
  bids_.emplace_back(GetUniqueID(), player_id, price, clock_->Time());
  std::ranges::sort(bids_, std::greater());
}

auto Market::GetBids() const -> std::vector<Offer> { return bids_; }

auto Market::GetStandingBid() const -> Offer {
  return *std::ranges::max_element(bids_);
}

auto Market::GetStandingAsk() const -> Offer {
  return *std::ranges::max_element(asks_);
}

auto Market::GetAsks() const -> std::vector<Offer> { return asks_; }

auto Market::SetClock(std::shared_ptr<Clock> clock) -> void { clock_ = clock; }

auto Market::AddAsk(unsigned int player_id, unsigned int price) -> void {
  asks_.emplace_back(GetUniqueID(), player_id, -price, clock_->Time());
  std::ranges::sort(asks_, std::greater());
}

auto Market::clear() -> void {
  bids_.clear();
  asks_.clear();
}

auto Market::GetUniqueID() -> unsigned int { return next_id++; }
