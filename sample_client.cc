#include <iostream>
#include <string>

#include "asio_client.hh"
#include "clientside_controller.hh"
#include "lib/asio-1.22.1/include/asio/io_context.hpp"
#include "src/clientside_processor.hh"
#include "src/pausable_timer.hh"

using namespace assetmarket;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: chat_client <host>\n";
      return 1;
    }

    asio::io_context context;
    auto client = std::make_shared<AsioClient>(context);
    auto state = std::make_shared<ClientState>();
    auto timer = std::make_shared<PausableTimer>(context);
    state->timer = timer;
    auto proc = std::make_unique<ClientsideProcessor>(state);
    auto cont = std::make_shared<ClientsideController>(client, std::move(proc));
    client->AddProcessor(cont);
    bool connected_to_server = client->Connect(argv[1], 12345);
    std::thread m_threadContext = std::thread([&context]() { context.run(); });
    while (connected_to_server) {
      std::string command;
      std::cin >> command;
      if (command == "test") {
        std::cout << "sending test message to server..."
                  << "\n";
        assetmarket::Message mess(MessageType::DebugMessage,
                                  "Test Message from Client");
        client->Send(mess);
      } else if (command == "bid") {
        int price;
        std::cin >> price;
        client->Send(Message(MessageType::Bid, to_string(price)));
      } else if (command == "ask") {
        int price;
        std::cin >> price;
        client->Send(Message(MessageType::Ask, to_string(price)));
      } else if (command == "retract") {
        unsigned int id;
        std::cin >> id;
        client->Send(Message(MessageType::Retract, to_string(id)));
      } else if (command == "create") {
        client->Send(Message(MessageType::Create));
      } else if (command == "ready") {
        client->Send(Message(MessageType::Ready));
      } else if (command == "name") {
        std::string name;
        getline(std::cin, name);
        client->Send(Message(MessageType::Name, name));
      }
    }
    client->Disconnect();
    context.stop();
    if (m_threadContext.joinable()) m_threadContext.join();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
