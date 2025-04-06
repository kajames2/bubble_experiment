#ifndef CLIENT_STATE_HH
#define CLIENT_STATE_HH


#include "configuration.hh"
#include "market_state.hh"
#include "offer_queue.hh"
#include "pausable_timer.hh"
#include "portfolio.hh"
#include "trade.hh"
#include "experiment_state.hh"

namespace assetmarket {

struct MarginRequirements {
  int min_marg;
  int max_marg;
};

struct ClientState : public MarketState {
  Portfolio start_round_port;
  Portfolio end_round_port;
  Portfolio post_dividend_port;
  Portfolio port;
  int last_dividend;
  RoundConfiguration round_conf;
  SubjectID id;
  SubjectConfiguration config;
  Subject subject;
  ExperimentState exp_state;
  std::shared_ptr<PausableTimer> timer;

};

}  // namespace assetmarket
#endif  // CLIENT_STATE_HH
