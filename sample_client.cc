#include <iostream>
#include <string>

#include "asio_client.hh"

using namespace assetmarket;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: chat_client <host>\n";
      return 1;
    }

    AsioClient client;
    bool connected_to_server = client.Connect(argv[1], 12345);
    std::cout << "Connected? " << connected_to_server << std::endl;

    while (connected_to_server) {
      std::string command;
      std::cin >> command;
      if (command == "test") {
        std::cout << "sending test message to server..."
                  << "\n";
        assetmarket::Message mess(MessageType::DebugMessage,
                                  "Test Message from Client");
        client.Send(mess);
      }
    }
    client.Disconnect();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
