#ifndef MESSAGE_HH
#define MESSAGE_HH

#include <string>

namespace assetmarket {
class Message {
 public:
  virtual auto Serialize() const -> std::string = 0;
  virtual auto Deserialize(std::string) -> void = 0;
  virtual ~Message(){};
};
}  // namespace assetmarket
#endif  // MESSAGE_HH
