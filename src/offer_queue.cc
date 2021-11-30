#include "offer_queue.hh"

#include <algorithm>

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
