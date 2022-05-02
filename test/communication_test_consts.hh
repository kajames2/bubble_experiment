#ifndef COMMUNICATION_TEST_CONSTS_HH
#define COMMUNICATION_TEST_CONSTS_HH

#include "message.hh"

namespace assettest {

static const assetmarket::Message TEST_MESSAGE(
    assetmarket::MessageType::DebugMessage, "Test Message");

}  // namespace assettest
#endif  // COMMUNICATION_TEST_CONSTS_HH
