#include <iostream>
#include <string>

#include "adminside_controller.hh"
#include "asio_client.hh"

using namespace assetmarket;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: chat_admin <host>\n";
      return 1;
    }

    asio::io_context context;
    auto admin = std::make_shared<AsioClient>(context);
    auto state = std::make_shared<AdminState>();
    auto timer = std::make_shared<PausableTimer>(context);
    state->timer = timer;
    auto admin_proc = std::make_unique<AdminsideProcessor>(state);
    auto cont =
        std::make_shared<AdminsideController>(admin, std::move(admin_proc));
    admin->AddProcessor(cont);
    bool connected_to_server = admin->Connect(argv[1], 12345);
    std::thread m_threadContext = std::thread([&context]() { context.run(); });

    while (connected_to_server) {
      std::string command;
      std::cin >> command;
      if (command == "StartExperiment") {
        admin->Send(Message(MessageType::StartExperiment));
      } else if (command == "StartInstructions") {
        admin->Send(Message(MessageType::StartInstructions));
      } else if (command == "EndEarly") {
        admin->Send(Message(MessageType::EndEarly));
      } else if (command == "LoadConfiguration") {
        std::string filepath;
        std::cin >> filepath;
        admin->Send(Message(MessageType::LoadConfiguration, filepath));
      }
    }
    admin->Disconnect();
    context.stop();
    if (m_threadContext.joinable()) m_threadContext.join();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
