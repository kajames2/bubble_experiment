#ifndef ADMINSIDE_PROCESSOR_HH
#define ADMINSIDE_PROCESSOR_HH

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "admin_state.hh"
#include "asset_market_enums.hh"
#include "configuration.hh"
#include "configuration_reader.hh"
#include "creation_result.hh"
#include "market_submission_result.hh"
#include "message.hh"
#include "retract_request.hh"
#include "server_message_processor.hh"
#include "string_funcs.hh"
#include "subject.hh"

namespace assetmarket {

class AdminsideProcessor {
 public:
  AdminsideProcessor(std::shared_ptr<AdminState> state) : state_(state) {}
  auto ProcessConnected() -> void {}
  auto ProcessDebugMessage(std::string msg) -> void {
    std::cerr << msg << std::endl;
  }
  auto ProcessSubjectConfiguration(SubjectConfiguration conf) -> void {
    state_->config = conf;
    std::cerr << PrettyPrint(conf);
  }
  auto ProcessStartExperiment(StartExperimentValidity val) -> void {
    std::cerr << PrettyPrint(val) << std::endl;
  }
  auto ProcessAcceptedOffer(MarketSubmissionResult res) -> void {
    if (res.validity != OfferValidity::Accepted) {
      std::cerr << "Accepted Offer is Invalid" << std::endl;
    }
    if (res.offer.price < 0) {
      state_->asks.push_back(res.offer);
    } else {
      state_->bids.push_back(res.offer);
    }
    if (res.trade) {
      state_->transactions.push_back(*res.trade);
      state_->asks.Retract(res.trade->ask_id);
      state_->bids.Retract(res.trade->bid_id);
    }
    for (const auto& ret : res.retracts) {
      state_->asks.Retract(ret.request.offer_id);
      state_->bids.Retract(ret.request.offer_id);
    }
    OnQueuesUpdated();
    OnTransactionsUpdated();
    OnMessage(PrettyPrint(res.validity));
  }

  auto ProcessRejectedOffer(MarketSubmissionResult res) -> void {
    if (res.validity == OfferValidity::Accepted) {
      std::cerr << "Rejected Offer is Valid" << std::endl;
    }
    OnMessage(PrettyPrint(res.validity));
  }

  auto ProcessCreationProcessed(CreationResult res) -> void {
    OnMessage(PrettyPrint(res.validity));
  }

  auto ProcessAcceptedRetraction(RetractResult rr) -> void {
    if (rr.validity != RetractValidity::Accepted) {
      std::cerr << "Accepted Retraction is Invalid" << std::endl;
    }
    state_->asks.Retract(rr.request.offer_id);
    state_->bids.Retract(rr.request.offer_id);
    OnQueuesUpdated();
    OnMessage(PrettyPrint(rr.validity));
  }
  auto ProcessRejectedRetraction(RetractResult rr) -> void {
    if (rr.validity == RetractValidity::Accepted) {
      std::cerr << "Rejected Retraction is Valid" << std::endl;
    }
    OnMessage(PrettyPrint(rr.validity));
  }
  auto ProcessPortfolio(SubjectPortfolio port) -> void {
    state_->player_ports[port.first] = port.second;
    OnPortfolioUpdated(port.first);
  }
  auto ProcessSubjectData(Subject s) -> void {
    state_->subjects[s.id] = s;
    OnSubjectUpdated(s.id);
  }

  auto ProcessTimeUpdate(Time time) -> void {
    state_->timer->Pause();
    state_->timer->UpdateTime(
        std::chrono::duration_cast<std::chrono::milliseconds>(time));
    OnTimeUpdate();
  }

  auto ProcessRoundStarted(RoundConfiguration conf) -> void {
    state_->asks.clear();
    state_->bids.clear();
    state_->transactions.clear();
    state_->round_conf = conf;
    state_->round = conf.round;
    OnQueuesUpdated();
    OnRoundStarted();
  }
  auto ProcessDividendStarted(int div) -> void {
    state_->last_dividend = div;
    OnMessage("Dividend Amount: " + std::to_string(div));
  }
  auto ProcessRoundEnded() -> void { OnRoundEnded(); }
  auto ProcessReviewStarted() -> void { OnReviewStarted(); }
  auto ProcessReviewEnded() -> void { OnReviewEnded(); }
  auto ProcessPayoutsStarted() -> void { OnPayoutsStarted(); }
  auto ProcessConfigurationLoaded(ConfigResult res) -> void {
    std::cerr << PrettyPrint(res) << std::endl;
  }
  auto ProcessSubjectReady(SubjectID id) -> void {}

  auto OnMessage(std::string mess) -> void { std::cerr << mess << std::endl; }

  auto OnQueuesUpdated() -> void {
    std::cout << "\nBids Queue\n";
    std::cout << PrettyPrint(state_->bids);
    std::cout << "\nAsks Queue\n";
    std::cout << PrettyPrint(state_->asks);
  }
  auto OnTransactionsUpdated() -> void {
    std::cout << "\nTransactions\nBuyer\tSeller\tPrice\tTime\n";
    for (const auto t : state_->transactions) {
      std::cout << PrettyPrint(t) << '\n';
    }
  }
  auto OnPortfolioUpdated(SubjectID id) -> void {
    std::cout << PrettyPrint(state_->player_ports[id]) << '\n';
    ;
  }
  auto OnSubjectUpdated(SubjectID id) -> void {
    std::cout << PrettyPrint(state_->subjects[id]) << '\n';
    ;
  }
  auto OnTimeUpdate() -> void {
    std::cout << "Time Remaining: "
              << static_cast<unsigned int>(
                     std::chrono::duration_cast<std::chrono::seconds>(
                         state_->timer->GetTimeRemaining())
                         .count())
              << std::endl;
  }

  auto OnRoundStarted() -> void {
    std::cout << PrettyPrint(state_->round_conf) << '\n';
    std::cout << "Round " << state_->round << " Started" << std::endl;
  }

  auto OnRoundEnded() -> void { std::cout << "Round Ended" << std::endl; }
  auto OnReviewStarted() -> void { std::cout << "Review Started" << std::endl; }
  auto OnReviewEnded() -> void { std::cout << "Review Ended" << std::endl; }
  auto OnPayoutsStarted() -> void {
    std::cout << "Payouts Started" << std::endl;
  }

 private:
  std::shared_ptr<AdminState> state_;
};

}  // namespace assetmarket
#endif  // ADMINSIDE_PROCESSOR_HH
