#ifndef MARKET_STATE_HH
#define MARKET_STATE_HH

#include <vector>

#include "offer_queue.hh"
#include "trade.hh"

namespace assetmarket {

struct MarketState {
  OfferQueue bids;
  OfferQueue asks;
  std::vector<Trade> transactions;
};

std::ostream& operator<<(std::ostream& os, const MarketState& ms);
std::istream& operator>>(std::istream& is, MarketState& ms);

}  // namespace assetmarket
#endif  // MARKET_STATE_HH
