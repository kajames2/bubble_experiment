#ifndef PORTFOLIO_H_
#define PORTFOLIO_H_

#include <exception>
#include <string>
#include <unordered_map>

namespace assetmarket {
class InvalidChangeByZero : std::exception {};

enum class Item { Cash, Shares };

class Portfolio {
 public:
  bool IsEmpty() { return item_count_.empty(); }
  void Add(Item item, unsigned int n) { Transact(item, static_cast<int>(n)); }

  void Subtract(Item item, unsigned int n) {
    Transact(item, -static_cast<int>(n));
  }

  int ItemCount(Item item) const {
    if (!item_count_.contains(item)) return 0;
    return item_count_.at(item);
  }

  bool operator==(const Portfolio& rhs) const = default;

 private:
  void Transact(Item item, int change) {
    if (change == 0) throw InvalidChangeByZero();
    item_count_[item] = ItemCount(item) + change;
  }

  std::unordered_map<Item, int> item_count_;
};
}  // namespace assetmarket
#endif  // PORTFOLIO_H_
