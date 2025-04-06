#ifndef OFFER_PROCESSOR_HH
#define OFFER_PROCESSOR_HH

#include "creation_result.hh"
#include "market_submission_result.hh"
#include "offer.hh"
#include "portfolio.hh"
#include "retract_request.hh"

namespace assetmarket {
class OfferProcessor {
 public:
  virtual auto ProcessOffer(Offer offer) -> MarketSubmissionResult = 0;
  virtual auto ProcessCreate(SubjectID id) -> CreationResult = 0;
  virtual auto ProcessRetract(RetractRequest rr) -> RetractResult = 0;
  virtual auto GetPortfolio(SubjectID id) const -> Portfolio = 0;
  virtual ~OfferProcessor(){};
};
}  // namespace assetmarket
#endif  // OFFER_PROCESSOR_HH
