#ifndef ADMIN_STATE_HH
#define ADMIN_STATE_HH

#include <vector>

#include "configuration.hh"
#include "market_state.hh"
#include "offer_queue.hh"
#include "pausable_timer.hh"
#include "portfolio.hh"
#include "trade.hh"

namespace exptemplate {

struct AdminState {
  unsigned int round;
  std::unordered_map<SubjectID, Subject> subjects;
  std::vector<SubjectID> ids;
  PortfolioSet player_ports;
};

}  // namespace exptemplate
#endif  // ADMIN_STATE_HH
