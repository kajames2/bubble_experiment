#ifndef OFFER_PROCESSOR_HH
#define OFFER_PROCESSOR_HH

#include "offer.hh"

class OfferProcessor {
 public:
  virtual auto ProcessOffer(Offer offer) -> void = 0;

  virtual ~OfferProcessor(){};

 private:
};

#endif  // OFFER_PROCESSOR_HH
