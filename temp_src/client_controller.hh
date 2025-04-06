#ifndef CLIENT_CONTROLLER_HH
#define CLIENT_CONTROLLER_HH

#include <memory>

#include "bid_ask.hh"
#include "clock.hh"
#include "offer.hh"
#include "offer_processor.hh"
#include "retract_request.hh"
#include "server.hh"
#include "subject.hh"

namespace assetmarket {
class ClientController {
 public:
  ClientController(std::unique_ptr<OfferProcessor> proc,
                   std::shared_ptr<Server> server,
                   std::shared_ptr<Clock> clock);

  auto TakeBid(BidAsk ba) -> void;
  auto TakeAsk(BidAsk ba) -> void;
  auto RetractOffer(RetractRequest rr) -> void;
  auto CreateAsset(SubjectID player_id) -> void;

 private:
  std::unique_ptr<OfferProcessor> proc_;
  std::shared_ptr<Server> server_;
  std::shared_ptr<Clock> clock_;
  unsigned int id = 0;

  auto TakeOffer(Offer offer) -> void;
  auto MakeOffer(SubjectID p_id, int price) -> Offer;
};

}  // namespace assetmarket
#endif  // CLIENT_CONTROLLER_HH
