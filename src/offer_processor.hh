#ifndef OFFER_PROCESSOR_HH
#define OFFER_PROCESSOR_HH

#include "market_submission_result.hh"
#include "offer.hh"

namespace assetmarket {
class OfferProcessor {
 public:
  virtual auto ProcessOffer(Offer offer) -> MarketSubmissionResult = 0;

  virtual ~OfferProcessor(){};
};
}  // namespace assetmarket
#endif  // OFFER_PROCESSOR_HH
