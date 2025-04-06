#ifndef EXPERIMENT_COMMAND_PROCESSOR_HH
#define EXPERIMENT_COMMAND_PROCESSOR_HH

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "asset_market_enums.hh"
#include "configuration.hh"
#include "configuration_reader.hh"
#include "experiment_interface.hh"
#include "market.hh"
#include "pausable_timer.hh"

namespace assetmarket {

class PeriodAssetProcessor {
 public:
  PeriodAssetProcessor(std::shared_ptr<Market> mark,
                       std::shared_ptr<PortfolioSet> ports)
      : market_(mark), portfolios_(ports) {}

  auto GetPortfolio(SubjectID id) const -> Portfolio {
    return portfolios_->at(id);
  }
  auto GetPortfolios() const -> PortfolioSet { return *portfolios_; }
  auto SetPortfolio(SubjectID id, Portfolio port) { (*portfolios_)[id] = port; }
  auto PayDividend(unsigned int dividend) -> void {
    for (auto& [sub, port] : *portfolios_) {
      unsigned int n_shares =
          static_cast<unsigned int>(port.ItemCount(Item::Shares));
      unsigned int n_derivs =
          static_cast<unsigned int>(port.ItemCount(Item::Derivatives));
      port.Add(Item::Cash, n_shares * dividend);
      port.Subtract(Item::Margin, n_derivs * dividend);
    }
  }
  auto ClearMarket() -> void { market_->clear(); }

 private:
  std::shared_ptr<Market> market_;
  std::shared_ptr<PortfolioSet> portfolios_;
};

class ExperimentCommandProcessor {
 public:
  ExperimentCommandProcessor(std::shared_ptr<Configuration> config,
                             std::shared_ptr<ExperimentInterface> exp,
                             std::shared_ptr<PausableTimer> timer,
                             std::unique_ptr<PeriodAssetProcessor> proc)
      : config_(config), exp_(exp), timer_(timer), proc_(std::move(proc)) {}

  auto ProcessStartExperiment() -> StartExperimentValidity {
    auto val = StartValidity();
    if (val == StartExperimentValidity::Accepted) {
      for (auto id : exp_->GetSubjectIDs()) {
        proc_->SetPortfolio(id,
                            config_->GetSubjectConfiguration(id).init_alloc);
      }
      exp_->StartExperiment();
    }
    return val;
  }

  auto ProcessStartRound() -> PausableTimer& {
    exp_->StartRound();
    timer_->Clear();
    timer_->SetDuration(config_->round_time);
    timer_->Start();
    prev_portfolios = proc_->GetPortfolios();
    return *timer_;
  }

  auto GetPortfolios() const -> PortfolioSet { return proc_->GetPortfolios(); }

  auto GetPortfolio(SubjectID id) const -> Portfolio {
    return proc_->GetPortfolio(id);
  }

  auto GetPortfolioDifference(SubjectID id) const -> Portfolio {
    return GetPortfolio(id).Diff(prev_portfolios.at(id));
  }

  auto GetSubjectIDs() const -> std::vector<SubjectID> {
    return exp_->GetSubjectIDs();
  }

  auto GetRoundConfiguration() const -> RoundConfiguration {
    return config_->GetRoundConfiguration(GetRound());
  }
  auto GetSubjectConfiguration(SubjectID id) const -> SubjectConfiguration {
    return config_->GetSubjectConfiguration(id);
  }

  auto GetSubject(SubjectID id) const -> Subject {
    return exp_->GetSubject(id);
  }
  auto ProcessEndRound() -> void {
    exp_->EndRound();
    proc_->ClearMarket();
  }

  auto ProcessStartDividends() -> unsigned int {
    unsigned int div = config_->GetRoundConfiguration(GetRound()).dividend;
    proc_->PayDividend(div);
    return div;
  }

  auto ProcessStartReview() -> PausableTimer& {
    exp_->StartReview();
    timer_->Clear();
    timer_->SetDuration(config_->review_time);
    timer_->Start();
    return *timer_;
  }

  auto ProcessEndReview() -> bool {
    exp_->EndReview();
    return (GetRound() < config_->n_rounds);
  }

  auto ProcessPause() -> void {
    exp_->Pause();
    timer_->Pause();
  }

  auto GetRound() const -> unsigned int { return exp_->GetRound(); }
  auto ProcessResume() -> void {
    exp_->Resume();
    timer_->Start();
  }

  auto ProcessStartPayouts() -> void {
    counter_ = 0;
    for (auto sub : exp_->GetSubjects()) {
      Portfolio port = GetPortfolio(sub.id);
      int payoff_cents = static_cast<int>(
          (port.ItemCount(Item::Cash) + port.ItemCount(Item::Margin)) * 100. /
          GetSubjectConfiguration(sub.id).eperdollar);
      exp_->SetPayoff(sub.id, payoff_cents);
    }
    exp_->StartPayouts();
  }

  auto ProcessEndExperiment() -> void { exp_->EndExperiment(); }

  auto ProcessStartInstructions() -> StartExperimentValidity {
    auto val = StartValidity();
    if (val == StartExperimentValidity::Accepted) {
      counter_ = 0;
      exp_->StartInstructions();
    }
    return val;
  }

  auto ProcessEndInstructions() -> void { exp_->EndInstructions(); }

  auto ProcessReady(SubjectID id) -> bool {
    counter_ += 1;
    return counter_ == exp_->GetSubjectIDs().size();
  }

  auto ProcessTakeName(SubjectID id, std::string name) -> bool {
    counter_ += 1;
    exp_->TakeName(id, name);
    bool done = counter_ == exp_->GetSubjectIDs().size();
    return done;
  }

  auto ProcessLoadConfiguration(std::string filepath) -> ConfigResult {
    Configuration config;
    ConfigResult res;
    auto read_res = ReadConfigurationFile(filepath);
    if (std::holds_alternative<ConfigError>(read_res)) {
      std::cout << "Had error..." << std::endl;
      ConfigError err = std::get<ConfigError>(read_res);
      res.error = err;
    } else {
      std::vector<ConfigWarning> warnings;
      std::tie(config, warnings) =
          std::get<std::pair<Configuration, std::vector<ConfigWarning>>>(
              read_res);
      res.warnings = warnings;
      exp_->SetConfiguration(config);
    }
    std::cout << PrettyPrint(res) << std::endl;
    return res;
  }

 private:
  auto StartValidity() const -> StartExperimentValidity {
    if (exp_->GetStatus() != Status::NotStarted) {
      return StartExperimentValidity::RejectedAlreadyStarted;
    }
    if (!config_ || config_->n_players == 0) {
      return StartExperimentValidity::RejectedNoConfig;
    }
    if (!(config_->n_players == exp_->GetSubjectIDs().size())) {
      return StartExperimentValidity::RejectedIncorrectPlayersCount;
    }
    return StartExperimentValidity::Accepted;
  }

  unsigned int counter_ = 0;
  PortfolioSet prev_portfolios;
  std::shared_ptr<Configuration> config_;
  std::shared_ptr<ExperimentInterface> exp_;
  std::shared_ptr<PausableTimer> timer_;
  std::unique_ptr<PeriodAssetProcessor> proc_;
};

}  // namespace assetmarket

#endif  // EXPERIMENT_COMMAND_PROCESSOR_HH
