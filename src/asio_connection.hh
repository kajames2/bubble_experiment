#ifndef ASIO_CONNECTION_HH
#define ASIO_CONNECTION_HH

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <deque>
#include <iostream>
#include <string>

#include "connection.hh"
#include "server.hh"

// The base of this code comes from
// raw.githubusercontent.com/Unlined/olcPixelGameEngine/master/Videos/Networking

namespace assetmarket {
class AsioConnection : public Connection {
 public:
  AsioConnection(
      asio::io_context& context, asio::ip::tcp::socket socket,
      std::function<void(Message)> do_handle_message,
      std::function<void(std::error_code)> do_handle_error =
          [](std::error_code) {},
      std::function<void(std::error_code)> do_handle_connection =
          [](std::error_code) {})
      : context_(context),
        socket_(std::move(socket)),
        do_handle_message_(do_handle_message),
        do_handle_error_(do_handle_error),
        do_handle_connection_(do_handle_connection) {}

  void ConnectToClient() {
    std::cout << socket_.remote_endpoint().address().to_string() << std::endl;
    std::cout << socket_.remote_endpoint().port() << std::endl;
    if (socket_.is_open()) {
      ReadHeader();
    }
  }

  void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
    asio::async_connect(socket_, endpoints,
                        [this](std::error_code ec, asio::ip::tcp::endpoint) {
                          do_handle_connection_(ec);
                          if (!ec) {
                            ReadHeader();
                          }
                        });
  }

  void Disconnect() {
    if (IsConnected()) asio::post(context_, [this]() { socket_.close(); });
  }

  bool IsConnected() const { return socket_.is_open(); }

  void Send(const Message& msg) {
    asio::post(context_, [this, msg]() {
      bool bWritingMessage = !m_qMessagesOut.empty();
      m_qMessagesOut.push_back(msg);
      if (!bWritingMessage) {
        WriteHeader();
      }
    });
  }

 private:
  void WriteHeader() {
    asio::async_write(
        socket_,
        asio::buffer(&m_qMessagesOut.front().header_, sizeof(MessageHeader)),
        [this](std::error_code ec, std::size_t) {
          if (!ec) {
            if (m_qMessagesOut.front().header_.size > 0) {
              WriteBody();
            } else {
              m_qMessagesOut.pop_front();
              if (!m_qMessagesOut.empty()) {
                WriteHeader();
              }
            }
          } else {
            do_handle_error_(ec);
            socket_.close();
          }
        });
  }

  void WriteBody() {
    asio::async_write(socket_,
                      asio::buffer(m_qMessagesOut.front().body_.data(),
                                   m_qMessagesOut.front().header_.size),
                      [this](std::error_code ec, std::size_t) {
                        if (!ec) {
                          m_qMessagesOut.pop_front();
                          if (!m_qMessagesOut.empty()) {
                            WriteHeader();
                          }
                        } else {
                          do_handle_error_(ec);
                          socket_.close();
                        }
                      });
  }

  void ReadHeader() {
    asio::async_read(
        socket_, asio::buffer(&m_msgTemporaryIn.header_, sizeof(MessageHeader)),
        [this](std::error_code ec, std::size_t) {
          if (!ec) {
            m_msgTemporaryIn.body_.resize(m_msgTemporaryIn.header_.size);
            if (m_msgTemporaryIn.header_.size > 0) {
              ReadBody();
            } else {
              do_handle_message_(m_msgTemporaryIn);
              ReadHeader();
            }
          } else {
            do_handle_error_(ec);
            socket_.close();
          }
        });
  }

  void ReadBody() {
    asio::async_read(socket_,
                     asio::buffer(m_msgTemporaryIn.body_.data(),
                                  m_msgTemporaryIn.body_.size()),
                     [this](std::error_code ec, std::size_t) {
                       if (!ec) {
                         do_handle_message_(m_msgTemporaryIn);
                         ReadHeader();
                       } else {
                         do_handle_error_(ec);
                         socket_.close();
                       }
                     });
  }

 protected:
  asio::io_context& context_;
  asio::ip::tcp::socket socket_;
  std::function<void(Message)> do_handle_message_;
  std::function<void(std::error_code)> do_handle_error_;
  std::function<void(std::error_code)> do_handle_connection_;

  Message m_msgTemporaryIn;
  std::deque<Message> m_qMessagesOut;
  bool m_bConnectionEstablished = false;
};
}  // namespace assetmarket

#endif  // ASIO_CONNECTION_HH
