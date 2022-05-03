#ifndef MOCK_SERVER
#define MOCK_SERVER

#include <unordered_map>
#include <vector>

#include "server.hh"

namespace assettest {
class MockServer : public assetmarket::Server {
 public:
  MockServer(int n_players) : counts_() {
    for (int i = 0; i < n_players; ++i) counts_.emplace(i, 0);
  }

  virtual auto Send(size_t id, const assetmarket::Message& message)
      -> void override {
    counts_.at(id) += 1;
    messages_.push_back(message);
  }

  virtual auto SendAll(const assetmarket::Message& message) -> void override {
    for (auto& [key, value] : counts_) {
      value += 1;
    }
    messages_.push_back(message);
  }

  virtual auto AddSubject(assetmarket::SubjectID id,
                          const assetmarket::ConnectionInfo& info)
      -> void override {
    counts_.emplace(id, 0);
  }

  std::vector<assetmarket::Message> messages_;
  std::unordered_map<assetmarket::SubjectID, int> counts_;
};
}  // namespace assettest
#endif  // MOCK_SERVER
