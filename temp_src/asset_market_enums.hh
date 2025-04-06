#ifndef ASSET_MARKET_ENUMS
#define ASSET_MARKET_ENUMS

#include <istream>
#include <ostream>
#include <string>

namespace assetmarket {
enum class ClientType { Subject, Admin };
enum class Status { NotStarted, Running, Paused, Ended };
enum class Stage {
  PreExperiment,
  Instructions,
  Round,
  Dividends,
  Review,
  Payouts,
  End
};
enum class StartExperimentValidity {
  Accepted,
  RejectedNoConfig,
  RejectedIncorrectPlayersCount,
  RejectedAlreadyStarted
};

enum class MessageType {
  // Client can send
  Bid,
  Ask,
  Retract,
  Create,
  Name,
  Ready,
  ClientType,

  // Client & server can send
  DebugMessage,
  SubjectConfiguration,

  // Server sends in reaction to clients
  SubjectID,
  AcceptedOffer,
  RejectedOffer,
  CreationProcessed,
  AcceptedRetraction,
  RejectedRetraction,
  MarketState,
  SubjectState,
  Portfolio,
  SubjectData,
  TimeUpdate,

  // Connections updates
  Connected,
  Disconnected,

  // Admin Sends
  StartInstructions,
  StartExperiment,
  EndEarly,
  LoadConfiguration,

  // Server Sends to Admins
  ConfigurationLoaded,
  SubjectReady,

  // Server Sends to Everyone
  ExperimentStarted,
  InstructionsStarted,
  Paused,
  Resumed,

  InstructionsEnded,
  ExperimentEnded,

  RoundStarted,
  RoundEnded,
  DividendStarted,
  DividendEnded,
  ReviewStarted,
  ReviewEnded,
  PayoutsStarted
};
std::string PrettyPrint(StartExperimentValidity val);
std::string PrettyPrint(MessageType type);
std::string PrettyPrint(Stage stage);
std::ostream& operator<<(std::ostream& os, StartExperimentValidity val);
std::istream& operator>>(std::istream& is, StartExperimentValidity& val);
}  // namespace assetmarket

#endif  // ASSET_MARKET_ENUMS
