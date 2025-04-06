#include <gtest/gtest.h>

#include <sstream>

#include "comms/message.hh"
#include "communication_test_consts.hh"

using namespace exptemplate::comms;
using namespace commstest;

TEST(AMessage, DefaultsToEmpty) {
  Message<TestMessage> message;
  EXPECT_EQ(message.header_.type, TestMessage::DebugMessage);
  EXPECT_EQ(message.header_.size, 0);
  EXPECT_EQ(message.body_, "");
}

TEST(AMessage, ProperlyCountsSizeWhenInitialized) {
  Message<TestMessage> message(TestMessage::DebugMessage, "Hello, World!");
  EXPECT_EQ(message.header_.size, 13);
}

TEST(AMessage, StoresMessageInBody) {
  Message<TestMessage> message(TestMessage::DebugMessage, "Hello, World!");
  EXPECT_EQ(message.header_.type, TestMessage::DebugMessage);
  EXPECT_EQ(message.body_, "Hello, World!");
}

TEST(AMessage, CanModifyBody) {
  Message<TestMessage> message;
  message.SetBody(TestMessage::DebugMessage, "Test Message");
  EXPECT_EQ(message.header_.type, TestMessage::DebugMessage);
  EXPECT_EQ(message.header_.size, 12);
  EXPECT_EQ(message.body_, "Test Message");
}

TEST(AMessage, ReadsInAndOut) {
  std::stringstream ss;
  ss << TEST_MESSAGE;
  EXPECT_EQ(ss.str(), "0;12;Test Message");

  Message<TestMessage> message;
  ss >> message;
  EXPECT_EQ(message.header_.type, TestMessage::DebugMessage);
  EXPECT_EQ(message.header_.size, 12);
  EXPECT_EQ(message.body_, "Test Message");
}
