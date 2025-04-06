#ifndef ADMINSIDE_CONTROLLER_HH
#define ADMINSIDE_CONTROLLER_HH

#include <iostream>
#include <memory>
#include <string>

#include "adminside_processor.hh"
#include "asio_client.hh"
#include "asset_market_enums.hh"
#include "client_state.hh"
#include "configuration.hh"
#include "configuration_reader.hh"
#include "creation_result.hh"
#include "market_submission_result.hh"
#include "message.hh"
#include "pausable_timer.hh"
#include "retract_request.hh"
#include "server_message_processor.hh"
#include "string_funcs.hh"

namespace assetmarket {

class AdminsideController : public ServerMessageProcessor {
 public:
  AdminsideController(std::shared_ptr<AsioClient> client,
                      std::unique_ptr<AdminsideProcessor> proc)
      : client_(client), proc_(std::move(proc)) {}

  auto Connected() -> void {
    client_->Send(Message(MessageType::ClientType,
                          std::to_string(static_cast<int>(ClientType::Admin))));
  }

  auto DebugMessage(std::string body) -> void {
    proc_->ProcessDebugMessage(body);
  }

  auto ReadSubjectConfiguration(std::string body) -> void {
    SubjectConfiguration conf;
    from_string(body, conf);
    proc_->ProcessSubjectConfiguration(conf);
    std::cerr << PrettyPrint(conf);
  }

  auto StartExperiment(std::string body) -> void {
    StartExperimentValidity val;
    from_string(body, val);
    proc_->ProcessStartExperiment(val);
  }
  auto ReadAcceptedOffer(std::string body) -> void {
    MarketSubmissionResult res;
    from_string(body, res);
    proc_->ProcessAcceptedOffer(res);
  }
  auto ReadRejectedOffer(std::string body) -> void {
    MarketSubmissionResult res;
    from_string(body, res);
    proc_->ProcessRejectedOffer(res);
  }
  auto ReadCreationProcessed(std::string body) -> void {
    CreationResult res;
    from_string(body, res);
    proc_->ProcessCreationProcessed(res);
  }
  auto ReadAcceptedRetraction(std::string body) -> void {
    RetractResult rr;
    from_string(body, rr);
    proc_->ProcessAcceptedRetraction(rr);
  }
  auto ReadRejectedRetraction(std::string body) -> void {
    RetractResult rr;
    from_string(body, rr);
    proc_->ProcessRejectedRetraction(rr);
  }
  auto ReadPortfolio(std::string body) -> void {
    SubjectPortfolio port;
    from_string(body, port);
    proc_->ProcessPortfolio(port);
  }
  auto ReadSubjectData(std::string body) -> void {
    Subject s;
    from_string(body, s);
    proc_->ProcessSubjectData(s);
  }
  auto ReadTimeUpdate(std::string body) -> void {
    Time rem;
    from_string(body, rem);
    proc_->ProcessTimeUpdate(rem);
  }

  auto ReadRoundStarted(std::string body) -> void {
    RoundConfiguration conf;
    from_string(body, conf);
    proc_->ProcessRoundStarted(conf);
  }
  auto ReadDividendStarted(std::string body) -> void {
    int div;
    from_string(body, div);
    proc_->ProcessDividendStarted(div);
  }
  auto ReadConfigurationLoaded(std::string body) -> void {
    ConfigResult conf;
    from_string(body, conf);
    proc_->ProcessConfigurationLoaded(conf);
  }
  auto ReadSubjectReady(std::string body) -> void {
    SubjectID id;
    from_string(body, id);
    proc_->ProcessSubjectReady(id);
  }

  auto ReadRoundEnded() -> void { proc_->ProcessRoundEnded(); }
  auto ReadReviewStarted() -> void { proc_->ProcessReviewStarted(); }
  auto ReadReviewEnded() -> void { proc_->ProcessReviewEnded(); }
  auto ReadPayoutsStarted() -> void { proc_->ProcessPayoutsStarted(); }

  auto ProcessMessage(Message message) -> void override {
    std::string body = message.body_;
    switch (message.header_.id) {
      case MessageType::Connected:
        Connected();
        break;
      case MessageType::DebugMessage:
        DebugMessage(body);
        break;
      case MessageType::SubjectConfiguration:
        ReadSubjectConfiguration(body);
        break;
      case MessageType::ExperimentStarted:
        StartExperiment(body);
        break;
      case MessageType::AcceptedOffer:
        ReadAcceptedOffer(body);
        break;
      case MessageType::RejectedOffer:
        ReadRejectedOffer(body);
        break;
      case MessageType::CreationProcessed:
        ReadCreationProcessed(body);
        break;
      case MessageType::AcceptedRetraction:
        ReadAcceptedRetraction(body);
        break;
      case MessageType::RejectedRetraction:
        ReadRejectedRetraction(body);
        break;
      case MessageType::Portfolio:
        ReadPortfolio(body);
        break;
      case MessageType::SubjectData:
        ReadSubjectData(body);
        break;
      case MessageType::TimeUpdate:
        ReadTimeUpdate(body);
        break;
      case MessageType::RoundStarted:
        ReadRoundStarted(body);
        break;
      case MessageType::RoundEnded:
        ReadRoundEnded();
        break;
      case MessageType::DividendStarted:
        ReadDividendStarted(body);
        break;
      case MessageType::ReviewStarted:
        ReadReviewStarted();
        break;
      case MessageType::ReviewEnded:
        ReadReviewEnded();
        break;
      case MessageType::PayoutsStarted:
        ReadPayoutsStarted();
        break;
      case MessageType::ConfigurationLoaded:
        ReadConfigurationLoaded(body);
        break;
      case MessageType::SubjectReady:
        ReadSubjectReady(body);
        break;
      default: {
        std::cerr << PrettyPrint(message.header_.id) << '\n';
        std::cerr << message.body_ << std::endl;
        break;
      }
    }
  }

 private:
  std::shared_ptr<AsioClient> client_;
  std::unique_ptr<AdminsideProcessor> proc_;
};

}  // namespace assetmarket

#endif  // ADMINSIDE_CONTROLLER_HH
