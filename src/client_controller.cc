#include "client_controller.hh"

ClientController::ClientController(std::unique_ptr<OfferProcessor> proc,
                                   std::shared_ptr<Clock> clock)
    : proc_(std::move(proc)), clock_(clock) {}

auto ClientController::TakeBid(unsigned int p_id, int price)
    -> MarketSubmissionResult {
  Offer offer = MakeOffer(p_id, price);
  return TakeOffer(offer);
}

auto ClientController::TakeAsk(unsigned int p_id, int price)
    -> MarketSubmissionResult {
  Offer offer = MakeOffer(p_id, -price);
  return TakeOffer(offer);
}

auto ClientController::TakeOffer(Offer offer) -> MarketSubmissionResult {
  return proc_->ProcessOffer(offer);
}

auto ClientController::MakeOffer(unsigned int p_id, int price) -> Offer {
  return Offer{id++, p_id, price, clock_->GetTime()};
}
