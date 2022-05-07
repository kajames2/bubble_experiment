#include "portfolio.hh"

namespace assetmarket {
bool Portfolio::IsEmpty() { return item_count_.empty(); }
void Portfolio::Add(Item item, unsigned int n) {
  Transact(item, static_cast<int>(n));
}

void Portfolio::Subtract(Item item, unsigned int n) {
  Transact(item, -static_cast<int>(n));
}

void Portfolio::Set(Item item, int n) { item_count_[item] = n; }

int Portfolio::ItemCount(Item item) const {
  if (!item_count_.contains(item)) return 0;
  return item_count_.at(item);
}

void Portfolio::Transact(Item item, int change) {
  if (change == 0) throw InvalidChangeByZero();
  item_count_[item] = ItemCount(item) + change;
}

std::ostream& operator<<(std::ostream& os, const Portfolio& port) {
  auto items = port.GetItems();
  os << items.size() << ";";
  unsigned int n = 0;
  for (auto [item, value] : items) {
    ++n;
    os << static_cast<int>(item) << ";" << value;
    if (n < items.size()) {
      os << ";";
    }
  }
  return os;
}

std::istream& operator>>(std::istream& is, Portfolio& port) {
  size_t n;
  is >> n;
  for (size_t i = 0; i < n; ++i) {
    int item_num;
    unsigned int count;
    is >> item_num;
    is >> count;
    Item item = static_cast<Item>(item_num);
    port.Add(item, count);
  }
  return is;
}
}  // namespace assetmarket
