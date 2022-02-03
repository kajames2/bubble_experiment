#ifndef OFFER_PROCESSOR_HH
#define OFFER_PROCESSOR_HH

#include "market_submission_result.hh"
#include "market_transaction.hh"
#include "offer.hh"

class OfferProcessor {
 public:
  virtual auto ProcessOffer(Offer offer) -> MarketSubmissionResult = 0;

  virtual ~OfferProcessor(){};
};

#endif  // OFFER_PROCESSOR_HH
