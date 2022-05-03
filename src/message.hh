#ifndef MESSAGE_HH
#define MESSAGE_HH

#include <istream>
#include <ostream>
#include <string>

namespace assetmarket {

// Adapted from
// https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/Networking/
enum class MessageType {
  AcceptedOffer,
  ExperimentStarted,
  DebugMessage,
  Connected,
  ClientType
};

struct MessageHeader {
  MessageType id{};
  size_t size = 0;
};

std::ostream& operator<<(std::ostream& os, MessageHeader header);
std::istream& operator>>(std::istream& is, MessageHeader header);

class Message {
 public:
  Message() {}
  Message(MessageType id, std::string in_str = "") { SetBody(id, in_str); }
  auto SetBody(MessageType id, std::string in_str) -> void {
    body_ = in_str;
    header_.id = id;
    header_.size = body_.size();
  }
  MessageHeader header_;
  std::string body_;
};

std::ostream& operator<<(std::ostream& os, Message message);

}  // namespace assetmarket
#endif  // MESSAGE_HH
