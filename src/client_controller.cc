#include "client_controller.hh"

#include "market_submission_result_message.hh"

namespace assetmarket {
ClientController::ClientController(std::unique_ptr<OfferProcessor> proc,
                                   std::shared_ptr<Server> server,
                                   std::shared_ptr<Clock> clock)
    : proc_(std::move(proc)), server_(server), clock_(clock) {}

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
  auto res = proc_->ProcessOffer(offer);
  server_->SendAll(MarketSubmissionResultMessage(res));
  return res;
}

auto ClientController::MakeOffer(unsigned int p_id, int price) -> Offer {
  return Offer{id++, p_id, price, clock_->GetTime()};
}
}  // namespace assetmarket
