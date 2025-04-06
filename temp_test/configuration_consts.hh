#ifndef CONFIGURATION_CONSTS_HH
#define CONFIGURATION_CONSTS_HH

#include "configuration.hh"
#include "offer_processor_market.hh"
#include "portfolio.hh"

namespace assettest {

auto InitAllocations() -> std::shared_ptr<assetmarket::PortfolioSet>;

static const assetmarket::Configuration TEST_CONFIG = {*InitAllocations()};

}  // namespace assettest
#endif  // CONFIGURATION_CONSTS_HH
