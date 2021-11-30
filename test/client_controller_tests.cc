#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "client_controller.hh"
#include "offer_processor.hh"
#include "portfolio.hh"

using namespace ::testing;

class AClientController : public Test {
 public:
  AClientController() {}

  ClientController cont_;
};

TEST_F(AClientController, DoesNotModifyPortfolioOnNonTrade) {}
