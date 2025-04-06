#include "portfolio.hh"

#include <sstream>
#include <string>

namespace exptemplate {
bool Portfolio::IsEmpty() { return item_count_.empty(); }
void Portfolio::Add(std::string item, unsigned int n) {
  Transact(item, static_cast<int>(n));
}

void Portfolio::Subtract(std::string item, unsigned int n) {
  Transact(item, -static_cast<int>(n));
}

void Portfolio::Set(std::string item, int n) { item_count_[item] = n; }

int Portfolio::ItemCount(std::string item) const {
  if (!item_count_.contains(item)) return 0;
  return item_count_.at(item);
}

void Portfolio::Transact(std::string item, int change) {
  item_count_[item] = ItemCount(item) + change;
}

auto Portfolio::Diff(const Portfolio& other) const -> Portfolio {
  Portfolio out = *this;
  for (const auto& [item, count] : other.GetItems()) {
    if (out.ItemCount(item) == 0) {
      out.Set(item, count);
    } else {
      out.item_count_[item] -= count;
    }
  }
  return out;
}

std::string PrettyPrint(std::string item) { return item; }

std::string PrettyPrint(const Portfolio& port) {
  std::stringstream ss;
  ss << "Portfolio:\n";
  for (auto [item, count] : port.GetItems()) {
    ss << '\t' + PrettyPrint(item) << ": " << count << '\n';
  }
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Portfolio& port) {
  auto items = port.GetItems();
  os << items.size() << " ";
  unsigned int n = 0;
  for (auto [item, value] : items) {
    ++n;
    os << item << " " << value;
    if (n < items.size()) {
      os << " ";
    }
  }
  return os;
}
std::ostream& operator<<(std::ostream& os, const SubjectPortfolio& port) {
  return os << port.first << " " << port.second;
}
std::istream& operator>>(std::istream& is, SubjectPortfolio& port) {
  return is >> port.first >> port.second;
}

std::istream& operator>>(std::istream& is, Portfolio& port) {
  unsigned int n;
  unsigned int count;
  std::string item;
  is >> n;
  for (unsigned int i = 0; i < n; ++i) {
    is >> item;
    is >> count;
    port.Add(item, count);
  }
  return is;
}
}  // namespace exptemplate
