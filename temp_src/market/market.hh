#ifndef MARKET_HH
#define MARKET_HH

#include <memory>
#include <ranges>
#include <tuple>

#include "clock.hh"
#include "offer.hh"
#include "offer_queue.hh"
#include "trade.hh"

namespace exptemplate {
namespace market {
class Market {
 public:
  Market() = default;
  Market(OfferQueue bids, OfferQueue asks);
  [[nodiscard]] auto IsEmpty() const -> bool;
  [[nodiscard]] auto Bids() const -> OfferQueue;
  [[nodiscard]] auto Asks() const -> OfferQueue;
  [[nodiscard]] auto StandingBid() const -> std::optional<Offer>;
  [[nodiscard]] auto StandingAsk() const -> std::optional<Offer>;
  [[nodiscard]] auto PeekPotentialTrade() const -> std::optional<Trade>;
  [[nodiscard]] auto PopTrade() -> std::optional<Trade>;
  auto GetOffer(unsigned int id) -> std::optional<Offer>;
  auto ProcessOffer(Offer offer) -> std::optional<Trade>;
  auto Retract(unsigned int id) -> void;
  auto clear() -> void;
  auto AddOffer(Offer offer) -> void;

 private:
  OfferQueue bids_;
  OfferQueue asks_;
};

auto CanTrade(const Offer& bid, const Offer& ask) -> bool;
}  // namespace market
}  // namespace exptemplate
#endif  // MARKET_HH
