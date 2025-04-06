#include "asset_market_enums.hh"

#include <istream>
#include <ostream>
#include <string>

namespace assetmarket {
std::string PrettyPrint(StartExperimentValidity val) {
  switch (val) {
    case StartExperimentValidity::Accepted:
      return "Starting Experiment";
    case StartExperimentValidity::RejectedAlreadyStarted:
      return "Cannot Start Experiment: Already Started";
    case StartExperimentValidity::RejectedIncorrectPlayersCount:
      return "Cannot Start Experiment: Wrong Player Count";
    case StartExperimentValidity::RejectedNoConfig:
      return "Cannot Start Experiment: No Configuration File Loaded";
    default:
      return "huh?";
  }
}
std::ostream& operator<<(std::ostream& os, StartExperimentValidity val) {
  return os << std::to_string(static_cast<int>(val));
}

std::istream& operator>>(std::istream& is, StartExperimentValidity& val) {
  int cval;
  is >> cval;
  val = static_cast<StartExperimentValidity>(cval);
  return is;
}

std::string PrettyPrint(Stage stage) {
    switch (stage) {
  case Stage::PreExperiment:
    return "Pre-Experiment";
  case Stage::Instructions:
    return "Instructions";
  case Stage::Round:
    return "Round";
  case Stage::Dividends:
    return "Dividends";
  case Stage::Review:
    return "Review";
  case Stage::Payouts:
    return "Payouts";
  case Stage::End:
    return "End";
    }
}
std::string PrettyPrint(MessageType type) {
  switch (type) {
    case MessageType::Bid:
      return "Bid";
    case MessageType::Ask:
      return "Ask";
    case MessageType::Retract:
      return "Retract";
    case MessageType::Create:
      return "Create";
    case MessageType::Name:
      return "Name";
    case MessageType::Ready:
      return "Ready";
    case MessageType::SubjectID:
      return "Subject ID";
    case MessageType::ClientType:
      return "Client Type";
    case MessageType::DebugMessage:
      return "Debug Message";
    case MessageType::SubjectConfiguration:
      return "Subject Configuration";
    case MessageType::AcceptedOffer:
      return "Accepted Offer";
    case MessageType::RejectedOffer:
      return "Rejected Offer";
    case MessageType::CreationProcessed:
      return "Creation Processed";
    case MessageType::AcceptedRetraction:
      return "Accepted Retraction";
    case MessageType::RejectedRetraction:
      return "Rejected Retraction";
    case MessageType::MarketState:
      return "Market State";
    case MessageType::SubjectState:
      return "Subject State";
    case MessageType::Portfolio:
      return "Portfolio";
    case MessageType::TimeUpdate:
      return "Time Update";
    case MessageType::Connected:
      return "Connected";
    case MessageType::Disconnected:
      return "Disconnected";
    case MessageType::StartInstructions:
      return "Start Instructions";
    case MessageType::StartExperiment:
      return "Start Experiment";
    case MessageType::EndEarly:
      return "End Early";
    case MessageType::LoadConfiguration:
      return "Load Configuration";
    case MessageType::ConfigurationLoaded:
      return "Configuration Loaded";
    case MessageType::SubjectReady:
      return "Subject Ready";
    case MessageType::SubjectData:
      return "Subject Data";
    case MessageType::ExperimentStarted:
      return "Experiment Started";
    case MessageType::InstructionsStarted:
      return "Instructions Started";
    case MessageType::Paused:
      return "Paused";
    case MessageType::Resumed:
      return "Resumed";
    case MessageType::InstructionsEnded:
      return "Instructions Ended";
    case MessageType::ExperimentEnded:
      return "Experiment Ended";
    case MessageType::RoundStarted:
      return "Round Started";
    case MessageType::RoundEnded:
      return "Round Ended";
    case MessageType::DividendStarted:
      return "Dividend Started";
    case MessageType::DividendEnded:
      return "Dividend Ended";
    case MessageType::ReviewStarted:
      return "Review Started";
    case MessageType::ReviewEnded:
      return "Review Ended";
    case MessageType::PayoutsStarted:
      return "Payouts Started";
    default:
      return "huh?";
  }
}
}  // namespace assetmarket
