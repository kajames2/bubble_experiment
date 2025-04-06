#ifndef MESSAGE_HH
#define MESSAGE_HH

#include <istream>
#include <ostream>
#include <string>

namespace exptemplate {
namespace comms {

// Adapted from
// https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/Networking/

template <typename T>
struct MessageHeader {
  T type{};
  size_t size = 0;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const MessageHeader<T> &header) {
  return os << static_cast<int>(header.type) << ";" << header.size;
}

template <typename T>
std::istream &operator>>(std::istream &is, MessageHeader<T> &header) {
  int type;
  is >> type;
  header.type = static_cast<T>(type);
  is.ignore();
  is >> header.size;
  return is;
}

template <typename T>
class Message {
 public:
  Message() {}
  Message(T type, std::string in_str = "") { SetBody(type, in_str); }
  auto SetBody(T type, std::string in_str) -> void {
    body_ = in_str;
    header_.type = type;
    header_.size = body_.size();
  }
  MessageHeader<T> header_;
  std::string body_;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Message<T> &message) {
  return os << message.header_ << ";" << message.body_;
}

template <typename T>
std::istream &operator>>(std::istream &is, Message<T> &message) {
  is >> message.header_;
  is.ignore();  // Ignore the semicolon
  std::getline(is, message.body_);
  return is;
}
}  // namespace comms
}  // namespace exptemplate
#endif  // MESSAGE_HH
