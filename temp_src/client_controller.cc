#include "client_controller.hh"

#include <algorithm>
#include <chrono>
#include <optional>
#include <string>

#include "portfolio.hh"
#include "retract_request.hh"
#include "string_funcs.hh"

namespace assetmarket {
ClientController::ClientController(std::unique_ptr<OfferProcessor> proc,
                                   std::shared_ptr<Server> server,
                                   std::shared_ptr<Clock> clock)
    : proc_(std::move(proc)), server_(server), clock_(clock) {}

auto ClientController::TakeBid(BidAsk ba) -> void {
  Offer offer = MakeOffer(ba.id, ba.price);
  TakeOffer(offer);
}

auto ClientController::TakeAsk(BidAsk ba) -> void {
  Offer offer = MakeOffer(ba.id, -ba.price);
  TakeOffer(offer);
}

auto ClientController::RetractOffer(RetractRequest rr) -> void {
  auto result = proc_->ProcessRetract(rr);
  if (result.validity == RetractValidity::Accepted) {
    server_->SendAll(
        Message(MessageType::AcceptedRetraction, to_string(result)));
    server_->SendAll(
        {MessageType::TimeUpdate, to_string(clock_->GetTimeRemaining())});
  } else {
    server_->Send(result.request.id,
                  Message(MessageType::RejectedRetraction, to_string(result)));
  }
}

auto ClientController::TakeOffer(Offer offer) -> void {
  auto res = proc_->ProcessOffer(offer);
  if (res.validity == OfferValidity::Accepted) {
    server_->SendAll({MessageType::AcceptedOffer, to_string(res)});
    server_->SendAll(
        {MessageType::TimeUpdate, to_string(clock_->GetTimeRemaining())});
  } else {
    server_->Send(res.offer.player_id,
                  {MessageType::RejectedOffer, to_string(res)});
  }
  if (res.trade) {
    SubjectID buyer = res.trade->buyer;
    server_->Send(buyer,
                  Message(MessageType::Portfolio,
                          to_string(SubjectPortfolio{
                              buyer, proc_->GetPortfolio(res.trade->buyer)})));
    SubjectID seller = res.trade->seller;
    server_->Send(
        seller, Message(MessageType::Portfolio,
                        to_string(SubjectPortfolio{
                            seller, proc_->GetPortfolio(res.trade->seller)})));
  }
}

auto ClientController::CreateAsset(SubjectID player_id) -> void {
  auto res = proc_->ProcessCreate(player_id);
  server_->Send(res.id, {MessageType::CreationProcessed, to_string(res)});
  if (res.validity == CreationValidity::Accepted) {
    server_->Send(
        res.id,
        {MessageType::Portfolio,
         to_string(SubjectPortfolio{res.id, proc_->GetPortfolio(res.id)})});
  }
}

auto ClientController::MakeOffer(SubjectID p_id, int price) -> Offer {
  return Offer{id++, p_id, price,
               static_cast<unsigned int>(
                   std::chrono::duration_cast<std::chrono::milliseconds>(
                       clock_->GetTimeElapsed())
                       .count())};
}

}  // namespace assetmarket
