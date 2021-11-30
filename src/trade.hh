#ifndef TRADE_HH
#define TRADE_HH

#include "offer.hh"

struct Trade {
  unsigned int buyer;
  unsigned int seller;
  unsigned int price;
  unsigned int timestamp;
  unsigned int bid_id;
  unsigned int ask_id;
};

#endif  // TRADE_HH
