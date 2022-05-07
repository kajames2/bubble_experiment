#include "message.hh"

#include <istream>
#include <ostream>

namespace assetmarket {

std::ostream& operator<<(std::ostream& os, const MessageHeader& header) {
  return os << static_cast<int>(header.id) << ";" << header.size;
}

std::istream& operator>>(std::istream& is, MessageHeader& header) {
  int id;
  is >> id;
  header.id = static_cast<MessageType>(id);
  is >> header.size;
  return is;
}

std::ostream& operator<<(std::ostream& os, const Message& message) {
  return os << message.header_ << ";" << message.body_;
}

}  // namespace assetmarket
