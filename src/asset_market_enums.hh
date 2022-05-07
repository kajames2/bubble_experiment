#ifndef ASSET_MARKET_ENUMS
#define ASSET_MARKET_ENUMS

namespace assetmarket {
enum class ClientType { Subject, Admin };
enum class Status { NotStarted, Running, Paused, Ended };
enum class Stage { PreExperiment, Instructions, Round, Review, Payouts, End };

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

  // Server sends in reaction to clients
  AcceptedOffer,
  RejectedOffer,
  CreationProcessed,
  AcceptedRetraction,
  RejectedRetraction,
  MarketState,
  SubjectState,
  Portfolio,
  Payout,

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
  SubjectData,

  // Server Sends to Everyone
  ExperimentStarted,
  InstructionsStarted,
  Paused,
  Resumed,

  InstructionsEnded,
  ExperimentEnded,

  RoundStarted,
  RoundEnded,
  ReviewStarted,
  ReviewEnded,
  PayoutsStarted
};
}  // namespace assetmarket

#endif  // ASSET_MARKET_ENUMS
