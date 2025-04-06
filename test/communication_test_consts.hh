#ifndef COMMUNICATION_TEST_CONSTS_HH
#define COMMUNICATION_TEST_CONSTS_HH

#include "comms/asio_server.hh"
#include "comms/message.hh"

namespace commstest {

enum class TestMessage : uint32_t { DebugMessage };
enum class DebugMessage : TestMessage,
    exptemplate::comms::BaseMessageType {
    } static const exptemplate::comms::Message<TestMessage>
        TEST_MESSAGE(TestMessage::DebugMessage, "Test Message");
}  // namespace commstest
#endif  // COMMUNICATION_TEST_CONSTS_HH
