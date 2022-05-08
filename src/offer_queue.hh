#ifndef OFFER_QUEUE_H
#define OFFER_QUEUE_H

#include <optional>
#include <vector>

#include "offer.hh"

namespace assetmarket {
class OfferQueue {
 public:
  void push_back(Offer offer);
  void pop_back() { offers_.pop_back(); }
  void clear() { offers_.clear(); }

  auto begin() const { return offers_.begin(); }
  auto end() const { return offers_.end(); }
  auto rbegin() const { return offers_.rbegin(); }
  auto rend() const { return offers_.rend(); }

  const Offer& operator[](unsigned int i) const { return offers_[i]; }
  Offer& operator[](unsigned int i) { return offers_[i]; }
  std::size_t size() const { return offers_.size(); }
  bool empty() const { return offers_.empty(); }

  std::vector<Offer> Offers(unsigned int player_id) const;
  void Retract(unsigned int id);
  std::optional<Offer> StandingOffer() const;
  std::optional<Offer> GetOffer(unsigned int id);

 private:
  std::vector<Offer> offers_;
};
}  // namespace assetmarket
#endif  // OFFER_QUEUE_H
