#ifndef PORTFOLIO_HH_
#define PORTFOLIO_HH_

#include <exception>
#include <istream>
#include <ostream>
#include <unordered_map>

namespace assetmarket {
class InvalidChangeByZero : std::exception {};

enum class Item { Cash, Shares, Derivatives, Margin };

class Portfolio {
 public:
  auto IsEmpty() -> bool;
  auto Add(Item item, unsigned int n) -> void;
  auto Subtract(Item item, unsigned int n) -> void;
  auto Set(Item item, int n) -> void;
  auto ItemCount(Item item) const -> int;
  auto GetItems() const -> std::unordered_map<Item, int> { return item_count_; }
  auto operator==(const Portfolio& rhs) const -> bool = default;

 private:
  auto Transact(Item item, int change) -> void;

  std::unordered_map<Item, int> item_count_;
};

std::ostream& operator<<(std::ostream& os, const Portfolio& port);
std::istream& operator>>(std::istream& os, Portfolio& port);
}  // namespace assetmarket
#endif  // PORTFOLIO_HH_
