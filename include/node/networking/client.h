#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "node/networking/net_protocol.h"

namespace Shabby::Node {

class Client {
public:
  Client() = default;
  ~Client();

  void Connect(const std::string& ip, int port);
  void Disconnect();
  void Send(const std::string& topic, const std::string& msg);

  void Poll();

  bool IsConnected() const;

  std::function<void()> OnConnected;
  std::function<void()> OnDisconnected;
  std::function<void(const std::string& topic, const std::string& message)> OnMessage;

private:
  void RecvLoop();

  struct InternalMessage {
    std::string topic;
    std::string message;
  };

  std::atomic<bool> _connected{false};
  int _fd = -1;
  std::mutex _send_mutex;

  std::mutex _queue_mutex;
  std::queue<InternalMessage> _incoming;

  std::thread _recv_thread;
};

} // namespace Shabby::Node

#endif // CLIENT_H
