#ifndef CLIENT_CONTROLLER_HH
#define CLIENT_CONTROLLER_HH

#include <memory>

#include "clock.hh"
#include "offer.hh"
#include "offer_processor.hh"

struct MarketSubmissionResult {
  MarketSubmissionStatus status;
  std::optional<Offer> offer;
};

class ClientController {
 public:
  ClientController(std::unique_ptr<OfferProcessor> proc)
      : proc_(std::move(proc)) {}
  // auto MakeOffer(unsigned int p_id, int price) -> Offer;

  auto TakeBid(unsigned int p_id, int price) -> MarketSubmissionResult {
    Offer offer = Offer{0, p_id, price, 0};
    auto res = proc_->ProcessOffer(offer);
    if (res != MarketSubmissionStatus::Accepted)
      return {res, std::nullopt};
    else
      return {res, offer};
  }
  // auto TakeAsk(unsigned int p_id, int price) -> void;

 private:
  std::unique_ptr<OfferProcessor> proc_;
  // std::shared_ptr<Clock> clock_;
  // unsigned int bid_id_;
};

#endif  // CLIENT_CONTROLLER_HH
