#include "configuration_consts.hh"

using namespace assetmarket;

namespace assettest {

auto InitAllocations() -> std::shared_ptr<PortfolioSet> {
  auto folio = std::make_shared<PortfolioSet>();
  for (unsigned int i = 0; i < 3; ++i) {
    folio->push_back(Portfolio());
  }
  (*folio)[0].Add(Item::Cash, 600);
  (*folio)[0].Add(Item::Shares, 1);
  (*folio)[1].Add(Item::Cash, 400);
  (*folio)[1].Add(Item::Shares, 2);
  (*folio)[2].Add(Item::Cash, 200);
  (*folio)[2].Add(Item::Shares, 3);
  return folio;
}

}  // namespace assettest
