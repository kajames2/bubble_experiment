#ifndef MARKET_HH
#define MARKET_HH

#include <algorithm>
#include <memory>
#include <ranges>
#include <tuple>
#include <vector>

#include "clock.hh"
#include "offer.hh"

class Market {
 public:
  [[nodiscard]] auto IsEmpty() const -> bool;
  [[nodiscard]] auto GetBids() const -> std::vector<Offer>;
  [[nodiscard]] auto GetAsks() const -> std::vector<Offer>;
  [[nodiscard]] auto GetStandingBid() const -> Offer;
  [[nodiscard]] auto GetStandingAsk() const -> Offer;
  auto AddBid(unsigned int player_id, unsigned int price) -> void;
  auto AddAsk(unsigned int player_id, unsigned int price) -> void;
  auto SetClock(std::shared_ptr<Clock> clock) -> void;
  auto clear() -> void;

 private:
  [[nodiscard]] auto GetUniqueID() -> unsigned int;
  std::vector<Offer> bids_;
  std::vector<Offer> asks_;
  unsigned int next_id = 0;
  std::shared_ptr<Clock> clock_;
};

#endif  // _MARKET_HH
