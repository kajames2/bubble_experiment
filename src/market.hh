#ifndef MARKET_HH
#define MARKET_HH

#include <algorithm>
#include <memory>
#include <ranges>
#include <tuple>
#include <vector>

#include "ask.hh"
#include "bid.hh"
#include "clock.hh"

class Market {
 public:
  [[nodiscard]] auto IsEmpty() const -> bool;
  [[nodiscard]] auto GetBids() const -> std::vector<Bid>;
  [[nodiscard]] auto GetAsks() const -> std::vector<Ask>;
  [[nodiscard]] auto GetStandingBid() const -> Bid;
  [[nodiscard]] auto GetStandingAsk() const -> Ask;
  auto AddBid(unsigned int player_id, unsigned int price) -> void;
  auto AddAsk(unsigned int player_id, unsigned int price) -> void;
  auto SetClock(std::shared_ptr<Clock> clock) -> void;
  auto clear() -> void;

 private:
  [[nodiscard]] auto GetUniqueID() -> unsigned int;
  std::vector<Bid> bids_;
  std::vector<Ask> asks_;
  unsigned int next_id = 0;
  std::shared_ptr<Clock> clock_;
};

#endif  // _MARKET_HH
