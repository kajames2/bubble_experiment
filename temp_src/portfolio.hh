#ifndef PORTFOLIO_HH_
#define PORTFOLIO_HH_

#include <exception>
#include <istream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "subject.hh"

namespace exptemplate {

class Portfolio {
 public:
  auto IsEmpty() -> bool;
  auto Add(std::string item, unsigned int n) -> void;
  auto Subtract(std::string item, unsigned int n) -> void;
  auto Set(std::string item, int n) -> void;
  auto ItemCount(std::string item) const -> int;
  auto GetItems() const -> std::unordered_map<std::string, int> {
    return item_count_;
  }
  auto operator==(const Portfolio& rhs) const -> bool = default;
  auto Diff(const Portfolio& p) const -> Portfolio;

 private:
  auto Transact(std::string item, int change) -> void;

  std::unordered_map<std::string, int> item_count_;
};

using PortfolioSet = std::unordered_map<SubjectID, Portfolio>;
using SubjectPortfolio = std::pair<SubjectID, Portfolio>;

std::string PrettyPrint(std::string item);
std::string PrettyPrint(const Portfolio& port);
std::ostream& operator<<(std::ostream& os, const Portfolio& port);
std::istream& operator>>(std::istream& os, Portfolio& port);
std::ostream& operator<<(std::ostream& os, const SubjectPortfolio& port);
std::istream& operator>>(std::istream& os, SubjectPortfolio& port);
}  // namespace exptemplate
#endif  // PORTFOLIO_HH_
