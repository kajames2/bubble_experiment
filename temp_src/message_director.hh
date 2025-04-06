#ifndef MESSAGE_DIRECTOR
#define MESSAGE_DIRECTOR

#include <string>

#include "client_controller.hh"
#include "client_message_processor.hh"
#include "connection_controller.hh"
#include "experiment_controller.hh"
#include "message.hh"
#include "string_funcs.hh"

namespace assetmarket {
class MessageDirector : public ClientMessageProcessor {
 public:
  MessageDirector(std::shared_ptr<ClientController> subject_cont,
                  std::shared_ptr<ExperimentController> exp_cont,
                  std::shared_ptr<ConnectionController> conn_cont)
      : subject_cont_(subject_cont),
        exp_cont_(exp_cont),
        conn_cont_(conn_cont) {}

  auto ProcessBid(size_t id, Message message) -> void {
    int price;
    from_string(message.body_, price);
    BidAsk ba{id, price};
    subject_cont_->TakeBid(ba);
  }

  auto ProcessAsk(size_t id, Message message) -> void {
    int price;
    from_string(message.body_, price);
    BidAsk ba{id, price};
    subject_cont_->TakeAsk(ba);
  }

  auto ProcessRetract(size_t id, Message message) -> void {
    unsigned int offer_id;
    from_string(message.body_, offer_id);
    RetractRequest rr{id, offer_id};
    subject_cont_->RetractOffer(rr);
  }

  auto ProcessCreate(size_t id, Message message) -> void {
    subject_cont_->CreateAsset(id);
  }

  auto ProcessName(size_t id, Message message) -> void {
    std::string name = message.body_;
    exp_cont_->TakeName(id, name);
  }

  auto ProcessReady(size_t id, Message message) -> void {
    exp_cont_->Ready(id);
  }

  auto ProcessClientType(size_t id, Message message) -> void {
    int cint;
    from_string(message.body_, cint);
    ClientType c = static_cast<ClientType>(cint);
    if (c == ClientType::Subject) {
      conn_cont_->AcceptSubject(id);
    } else if (c == ClientType::Admin) {
      conn_cont_->AcceptAdmin(id);
    }
  }

  auto ProcessStartInstructions(Message message) -> void {
    exp_cont_->StartInstructions();
  }

  auto ProcessStartExperiment(Message message) -> void {
    exp_cont_->StartExperiment();
  }

  auto ProcessEndEarly(Message message) -> void { exp_cont_->StartPayouts(); }

  auto ProcessLoadConfiguration(Message message) -> void {
    std::string filepath = message.body_;
    exp_cont_->LoadConfiguration(filepath);
  }

  auto ProcessMessage(size_t conn_id, Message message) -> void override {
    switch (message.header_.id) {
      case MessageType::StartInstructions:
        ProcessStartInstructions(message);
        break;
      case MessageType::StartExperiment:
        ProcessStartExperiment(message);
        break;
      case MessageType::EndEarly:
        ProcessEndEarly(message);
        break;
      case MessageType::LoadConfiguration:
        ProcessLoadConfiguration(message);
        break;
      case MessageType::ClientType:
        ProcessClientType(conn_id, message);
        break;
      default:
        break;
    }
  }

  auto ProcessSubjectMessage(SubjectID id, Message message) -> void override {
    switch (message.header_.id) {
      case MessageType::Bid:
        ProcessBid(id, message);
        break;
      case MessageType::Ask:
        ProcessAsk(id, message);
        break;
      case MessageType::Retract:
        ProcessRetract(id, message);
        break;
      case MessageType::Create:
        ProcessCreate(id, message);
        break;
      case MessageType::Name:
        ProcessName(id, message);
        break;
      case MessageType::Ready:
        ProcessReady(id, message);
        break;
      default:
        break;
    }
  }

 private:
  std::shared_ptr<ClientController> subject_cont_;
  std::shared_ptr<ExperimentController> exp_cont_;
  std::shared_ptr<ConnectionController> conn_cont_;
};
}  // namespace assetmarket

#endif  // MESSAGE_DIRECTOR
