#ifndef CLIENT_CONTROLLER_HH
#define CLIENT_CONTROLLER_HH

#include <memory>

#include "clock.hh"
#include "offer.hh"
#include "offer_processor.hh"

class ClientController {
 public:
  ClientController(std::unique_ptr<OfferProcessor> proc,
                   std::shared_ptr<Clock> clock);

  auto TakeBid(unsigned int p_id, int price) -> MarketSubmissionResult;
  auto TakeAsk(unsigned int p_id, int price) -> MarketSubmissionResult;

 private:
  std::unique_ptr<OfferProcessor> proc_;
  std::shared_ptr<Clock> clock_;
  unsigned int id = 0;

  auto TakeOffer(Offer offer) -> MarketSubmissionResult;
  auto MakeOffer(unsigned int p_id, int price) -> Offer;
};

#endif  // CLIENT_CONTROLLER_HH
