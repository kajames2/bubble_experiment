#include <iostream>
#include <string>

#include "asio_client.hh"
#include "clientside_controller.hh"

using namespace assetmarket;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: chat_client <host>\n";
      return 1;
    }

    auto client = std::make_shared<AsioClient>();
    auto cont = std::make_shared<ClientsideController>(client);
    client->AddProcessor(cont);
    bool connected_to_server = client->Connect(argv[1], 12345);

    while (connected_to_server) {
      std::string command;
      std::cin >> command;
      if (command == "test") {
        std::cout << "sending test message to server..."
                  << "\n";
        assetmarket::Message mess(MessageType::DebugMessage,
                                  "Test Message from Client");
        client->Send(mess);
      }
    }
    client->Disconnect();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
