#ifndef MESSAGE_HH
#define MESSAGE_HH

#include <ostream>
#include <string>

namespace assetmarket {

// Adapted from
// https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/Networking/
enum class MessageType { AcceptedOffer };

struct MessageHeader {
  MessageType id{};
  size_t size = 0;
};

class Message {
 public:
  virtual auto Serialize() const -> std::string = 0;
  virtual auto Deserialize(const std::string&) -> void = 0;
  virtual ~Message(){};
};
}  // namespace assetmarket
#endif  // MESSAGE_HH
