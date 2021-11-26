#ifndef PORTFOLIO_H_
#define PORTFOLIO_H_

#include <exception>
#include <string>
#include <unordered_map>

class InvalidChangeByZero : std::exception {};

class Portfolio {
 public:
  bool IsEmpty() { return item_count_.empty(); }
  void Add(const std::string& item, unsigned int n) {
    Transact(item, static_cast<int>(n));
  }

  void Subtract(const std::string& item, unsigned int n) {
    Transact(item, -static_cast<int>(n));
  }

  int ItemCount(const std::string& item) const {
    if (!item_count_.contains(item)) return 0;
    return item_count_.at(item);
  }

 private:
  void Transact(const std::string& item, int change) {
    if (change == 0) throw InvalidChangeByZero();
    item_count_[item] = ItemCount(item) + change;
  }

  std::unordered_map<std::string, int> item_count_;
};

#endif  // PORTFOLIO_H_
