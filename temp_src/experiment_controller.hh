#ifndef EXPERIMENT_CONTROLLER_HH
#define EXPERIMENT_CONTROLLER_HH

#include <chrono>
#include <memory>
#include <string>

#include "asset_market_enums.hh"
#include "connection_info.hh"
#include "experiment_command_processor.hh"
#include "server.hh"
#include "string_funcs.hh"

namespace assetmarket {

class ExperimentController {
 public:
  ExperimentController(std::shared_ptr<Server> serv,
                       std::unique_ptr<ExperimentCommandProcessor> proc)
      : serv_(serv), proc_(std::move(proc)) {}

  auto StartExperiment() -> void {
    StartExperimentValidity val = proc_->ProcessStartExperiment();
    serv_->SendAdmins(Message(MessageType::ExperimentStarted, to_string(val)));
    if (val == StartExperimentValidity::Accepted) {
      for (auto id : proc_->GetSubjectIDs()) {
        serv_->Send(id, Message(MessageType::SubjectConfiguration,
                                to_string(proc_->GetSubjectConfiguration(id))));
        serv_->Send(
            id,
            Message(MessageType::Portfolio,
                    to_string(SubjectPortfolio{id, proc_->GetPortfolio(id)})));
      }
      StartRound();
    }
  }

  auto StartRound() -> void {
    PausableTimer& timer = proc_->ProcessStartRound();
    timer.Register([this]() { EndRound(); });
    serv_->SendAll(Message(MessageType::RoundStarted,
                           to_string(proc_->GetRoundConfiguration())));
    Time t = timer.GetTimeRemaining();
    serv_->SendAll({MessageType::TimeUpdate, to_string(t)});
  }

  auto EndRound() -> void {
    proc_->ProcessEndRound();
    serv_->SendAll(Message(MessageType::RoundEnded));
    StartDividends();
  }

  auto StartDividends() -> void {
    auto dividend = proc_->ProcessStartDividends();
    serv_->SendAll(Message(MessageType::DividendStarted, to_string(dividend)));
    for (auto id : proc_->GetSubjectIDs()) {
      serv_->Send(
          id,
          Message(MessageType::Portfolio,
                  to_string(SubjectPortfolio{id, proc_->GetPortfolio(id)})));
    }
    StartReview();
  }

  auto StartReview() -> void {
    auto& timer = proc_->ProcessStartReview();
    timer.Register([this]() { EndReview(); });
    serv_->SendAll(Message(MessageType::ReviewStarted));
    serv_->SendAll(
        {MessageType::TimeUpdate, to_string(timer.GetTimeRemaining())});
  }

  auto EndReview() -> void {
    bool another_round = proc_->ProcessEndReview();
    serv_->SendAll(Message(MessageType::ReviewEnded));
    if (another_round) {
      StartRound();
    } else {
      StartPayouts();
    }
  }

  auto Pause() -> void {
    proc_->ProcessPause();
    serv_->SendAll(Message(MessageType::Paused));
  }

  auto Resume() -> void {
    proc_->ProcessResume();
    serv_->SendAll(Message(MessageType::Resumed));
  }

  auto StartPayouts() -> void {
    proc_->ProcessStartPayouts();
    serv_->SendAll(Message(MessageType::PayoutsStarted));
  }

  auto EndExperiment() -> void {
    proc_->ProcessEndExperiment();
    serv_->SendAll(Message(MessageType::ExperimentEnded));
  }

  auto StartInstructions() -> void {
    auto val = proc_->ProcessStartInstructions();
    serv_->SendAdmins(
        Message(MessageType::InstructionsStarted, to_string(val)));
    if (val == StartExperimentValidity::Accepted) {
      for (auto id : proc_->GetSubjectIDs()) {
        serv_->Send(id, Message(MessageType::SubjectConfiguration,
                                to_string(proc_->GetSubjectConfiguration(id))));
      }
      serv_->SendAll(Message(MessageType::InstructionsStarted));
    }
  }

  auto EndInstructions() -> void {
    proc_->ProcessEndInstructions();
    serv_->SendAll(Message(MessageType::InstructionsEnded));
  }

  auto Ready(SubjectID id) -> void {
    bool done = proc_->ProcessReady(id);
    serv_->SendAdmins(Message(MessageType::SubjectReady, to_string(id)));
    if (done) {
      EndInstructions();
    }
  }

  auto TakeName(SubjectID id, std::string name) -> void {
    bool done = proc_->ProcessTakeName(id, name);
    serv_->Send(id, Message(MessageType::SubjectData,
                            to_string(proc_->GetSubject(id))));
    if (done) {
      EndExperiment();
    }
  }

  auto LoadConfiguration(std::string filepath) -> void {
    auto res = proc_->ProcessLoadConfiguration(filepath);
    serv_->SendAdmins(
        Message(MessageType::ConfigurationLoaded, to_string(res)));
  }

 private:
  std::shared_ptr<Server> serv_;
  std::unique_ptr<ExperimentCommandProcessor> proc_;
};

}  // namespace assetmarket

#endif  // EXPERIMENT_CONTROLLER_HH
