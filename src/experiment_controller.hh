#ifndef EXPERIMENT_CONTROLLER_HH
#define EXPERIMENT_CONTROLLER_HH

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "asset_market_enums.hh"
#include "configuration.hh"
#include "configuration_reader.hh"
#include "connection_info.hh"
#include "experiment_interface.hh"
#include "market_submission_result.hh"
#include "pausable_timer.hh"
#include "server.hh"
#include "string_funcs.hh"

namespace assetmarket {

class ExperimentController {
 public:
  ExperimentController(std::shared_ptr<Server> serv,
                       std::shared_ptr<ExperimentInterface> exp,
                       std::shared_ptr<PausableTimer> timer)
      : serv_(serv), exp_(exp), timer_(timer) {}

  auto StartExperiment() -> void {
    exp_->StartExperiment();
    SendPortfolios();
    StartRound();
  }

  auto SendPortfolios() -> void {
    for (auto id : exp_->GetSubjectIDs()) {
      auto subject_config = config_.GetSubjectConfiguration(id);
      serv_->Send(id, Message(MessageType::Portfolio,
                              to_string(subject_config.init_alloc)));
    }
  }

  auto StartRound() -> void {
    exp_->StartRound();
    timer_->Clear();
    timer_->SetDuration(config_.round_time);
    timer_->Register([this]() { EndRound(); });
    timer_->Start();
    serv_->SendAll(Message(MessageType::RoundStarted));
  }

  auto EndRound() -> void {
    exp_->EndRound();
    serv_->SendAll(Message(MessageType::RoundEnded));
    // for (auto sub : exp_->GetSubjects()) {
    // }
    StartReview();
  }

  auto StartReview() -> void {
    exp_->StartReview();
    timer_->Clear();
    timer_->SetDuration(config_.review_time);
    timer_->Register([this]() { EndReview(); });
    timer_->Start();
    serv_->SendAll(Message(MessageType::ReviewStarted));
  }

  auto EndReview() -> void {
    exp_->EndReview();
    serv_->SendAll(Message(MessageType::ReviewEnded));
    if (exp_->GetRound() == config_.n_rounds) {
      StartPayouts();
    } else {
      StartRound();
    }
  }

  auto Pause() -> void {
    exp_->Pause();
    timer_->Pause();
    serv_->SendAll(Message(MessageType::Paused));
  }

  auto Resume() -> void {
    exp_->Resume();
    timer_->Start();
    serv_->SendAll(Message(MessageType::Resumed));
  }

  auto StartPayouts() -> void {
    exp_->StartPayouts();
    serv_->SendAll(Message(MessageType::PayoutsStarted));
  }

  auto EndExperiment() -> void {
    exp_->EndExperiment();
    serv_->SendAll(Message(MessageType::ExperimentEnded));
  }

  auto StartInstructions() -> void {
    exp_->StartInstructions();
    for (auto id : exp_->GetSubjectIDs()) {
      bool is_creator = config_.GetSubjectConfiguration(id).is_creator;
      serv_->Send(id, Message(MessageType::InstructionsStarted,
                              std::to_string(is_creator)));
    }
  }

  auto EndInstructions() -> void {
    exp_->EndInstructions();
    serv_->SendAll(Message(MessageType::InstructionsEnded));
  }

  auto Ready(SubjectID id) -> void {
    serv_->SendAdmins(Message(MessageType::SubjectReady, to_string(id)));
  }

  auto TakeName(SubjectID id, std::string name) -> void {
    exp_->TakeName(id, name);
    serv_->SendAdmins(
        Message(MessageType::SubjectData, to_string(exp_->GetSubject(id))));
  }

  auto LoadConfiguration(std::string filepath) -> void {
    Configuration config;
    ConfigResult res;
    auto read_res = ReadConfigurationFile(filepath);
    if (std::holds_alternative<ConfigError>(read_res)) {
      *res.error = std::get<ConfigError>(read_res);
    } else {
      std::vector<ConfigWarning> warnings;
      std::tie(config, warnings) =
          std::get<std::pair<Configuration, std::vector<ConfigWarning>>>(
              read_res);
      res.warnings = warnings;
      exp_->SetConfiguration(config);
    }
    std::cerr << PrettyPrint(res) << std::endl;
    serv_->SendAdmins(
        Message(MessageType::ConfigurationLoaded, to_string(res)));
  }

 private:
  Configuration config_;
  std::shared_ptr<Server> serv_;
  std::shared_ptr<ExperimentInterface> exp_;
  std::shared_ptr<PausableTimer> timer_;
};

}  // namespace assetmarket

#endif  // EXPERIMENT_CONTROLLER_HH
