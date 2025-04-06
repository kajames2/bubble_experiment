#include "offer_queue.hh"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace assetmarket {
void OfferQueue::push_back(Offer offer) {
  offers_.push_back(offer);
  std::ranges::sort(offers_);
}

void OfferQueue::Retract(unsigned int id) {
  std::erase_if(offers_, [id](const Offer& off) { return off.id == id; });
}

std::vector<Offer> OfferQueue::Offers(unsigned int player_id) const {
  std::vector<Offer> offers;
  std::copy_if(
      offers_.begin(), offers_.end(), std::back_inserter(offers),
      [player_id](const Offer& b) { return b.player_id == player_id; });
  return offers;
}

std::optional<Offer> OfferQueue::StandingOffer() const {
  return empty() ? std::nullopt : std::optional<Offer>(offers_.back());
}

std::optional<Offer> OfferQueue::GetOffer(unsigned int id) {
  auto offer = std::find_if(offers_.begin(), offers_.end(),
                            [id](const Offer& b) { return b.id == id; });
  return offer == offers_.end() ? std::nullopt : std::optional<Offer>(*offer);
}

std::string PrettyPrint(const OfferQueue& q) {
  std::stringstream ss;
  ss << "OffID\tPlayID\tPrice\tTime\n";
  for (const auto& offer : q) {
    ss << PrettyPrint(offer) << "\n";
  }
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const OfferQueue& q) {
  os << q.size();
  for (auto& off : q) {
    os << " " << off;
  }
  return os;
}
std::istream& operator>>(std::istream& is, OfferQueue& q) {
  int size;
  is >> size;
  for (int i = 0; i < size; ++i) {
    Offer o;
    is >> o;
    q.push_back(o);
  }
  return is;
}

}  // namespace assetmarket
