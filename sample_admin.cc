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

    auto admin = std::make_shared<AsioClient>();
    auto cont = std::make_shared<AdminsideController>(admin);
    admin->AddProcessor(cont);
    bool connected_to_server = admin->Connect(argv[1], 12345);

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
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
