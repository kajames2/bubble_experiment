#include <iostream>
#include <string>

#include "asio_server.hh"
#include "src/client_message_processor.hh"

using namespace assetmarket;

class MockMessageProcessor : public ClientMessageProcessor {
  auto ProcessMessage(size_t id, Message message) -> void override {
    std::cout << id << " " << message.header_ << " " << message.body_ << "\n";
  }
};

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: server <port>\n";
    return 1;
  }

  auto proc = std::make_shared<MockMessageProcessor>();
  AsioServer server(12345);
  server.AddProcessor(proc);
  server.Start();

  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "test") {
      std::cout << "sending test message to clients..."
                << "\n";
      assetmarket::Message mess(MessageType::DebugMessage,
                                "Test Message from Server");
      server.SendAll(mess);
    } else if (command == "stop") {
      server.Stop();
    } else {
      std::cout << "Invalid command" << std::endl;
    }
    std::cin.ignore();
  }
  server.Stop();
}
