#ifndef MESSAGE_HH
#define MESSAGE_HH

#include <string>

namespace assetmarket {
class Message {
  virtual auto Serialize() -> std::string = 0;

  virtual ~Message(){};
};
}  // namespace assetmarket
#endif  // MESSAGE_HH
