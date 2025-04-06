#include <iostream>
#include <string>

#include "asio_server.hh"
#include "client_message_processor.hh"
#include "connection_controller.hh"
#include "experiment.hh"
#include "experiment_command_processor.hh"
#include "market.hh"
#include "message_director.hh"
#include "offer_processor_market.hh"

using namespace assetmarket;

class MockMessageProcessor : public ClientMessageProcessor {
  auto ProcessMessage(size_t conn_id, Message message) -> void override {
    std::cout << "Non-Subject Message: CID=" << conn_id << " "
              << message.header_ << " " << message.body_ << "\n";
  }
  auto ProcessSubjectMessage(SubjectID id, Message message) -> void override {
    std::cout << "Subject Message: SID=" << id << " " << message.header_ << " "
              << message.body_ << "\n";
  }
};

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: server <port>\n";
    return 1;
  }

  asio::io_context context;

  auto proc = std::make_shared<MockMessageProcessor>();
  auto server = std::make_shared<AsioServer>(context, 12345);
  auto exp_state = std::make_shared<ExperimentState>();
  auto market = std::make_shared<Market>();
  auto config = std::make_shared<Configuration>();
  auto exp = std::make_shared<Experiment>(config, exp_state);
  auto timer = std::make_shared<PausableTimer>(context);
  auto set = std::make_shared<PortfolioSet>();
  auto asset_proc = std::make_unique<PeriodAssetProcessor>(market, set);
  auto exp_proc = std::make_unique<ExperimentCommandProcessor>(
      config, exp, timer, std::move(asset_proc));
  auto offer_proc =
      std::make_unique<OfferProcessorMarket>(market, set, exp_state, config);
  auto conn_cont = std::make_shared<ConnectionController>(server, exp);
  auto exp_cont =
      std::make_shared<ExperimentController>(server, std::move(exp_proc));
  auto client_cont =
      std::make_shared<ClientController>(std::move(offer_proc), server, timer);
  auto director =
      std::make_shared<MessageDirector>(client_cont, exp_cont, conn_cont);

  server->AddProcessor(proc);
  server->AddProcessor(director);
  server->Start();
  std::thread m_threadContext = std::thread([&context]() { context.run(); });
  bool stopped = false;
  while (!stopped) {
    std::string command;
    std::cin >> command;
    if (command == "testall") {
      std::cout << "sending test message to clients..."
                << "\n";
      assetmarket::Message mess(MessageType::DebugMessage,
                                "Test Message from Server");
      server->SendAll(mess);
    } else if (command == "test") {
      size_t subject;
      std::cin >> subject;
      server->Send(subject,
                   {MessageType::DebugMessage,
                    "Test Message to Subject " + std::to_string(subject)});
    } else if (command == "stop") {
      server->Stop();
      stopped = true;
    } else {
      std::cout << "Invalid command" << std::endl;
    }
    std::cin.ignore();
  }
  context.stop();
  if (m_threadContext.joinable()) m_threadContext.join();
}
