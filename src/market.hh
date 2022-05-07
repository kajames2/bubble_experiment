#ifndef MARKET_HH
#define MARKET_HH

#include <memory>
#include <ranges>
#include <tuple>

#include "clock.hh"
#include "offer.hh"
#include "offer_queue.hh"
#include "trade.hh"

namespace assetmarket {
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
  auto ProcessOffer(Offer offer) -> std::optional<Trade>;
  auto Retract(unsigned int id) -> void;
  auto clear() -> void;
  auto AddOffer(Offer offer) -> void;

 private:
  OfferQueue bids_;
  OfferQueue asks_;
};
}  // namespace assetmarket
#endif  // MARKET_HH
