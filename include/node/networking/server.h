#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "node/networking/net_protocol.h"

namespace Shabby::Node {

struct NetworkMessage {
  int client_id;
  std::string topic;
  std::string message;
};

class Server {
public:
  Server() = default;
  ~Server();

  void Start(int port);
  void Stop();

  void Broadcast(const std::string& topic, const std::string& msg);
  void SendToClient(int client_id, const std::string& topic, const std::string& msg);

  void Poll();

  bool IsRunning() const;

  std::function<void(int client_id)> OnClientConnected;
  std::function<void(int client_id)> OnClientDisconnected;
  std::function<void(int client_id, const std::string& topic, const std::string& message)> OnMessage;

private:
  void AcceptLoop();
  void ClientRecvLoop(int client_id, int fd);

  std::atomic<bool> _running{false};
  int _port = 0;
  int _server_fd = -1;
  int _next_client_id = 1;

  std::mutex _clients_mutex;
  std::unordered_map<int, int> _client_fds;

  std::mutex _queue_mutex;
  std::queue<NetworkMessage> _incoming;

  std::thread _accept_thread;
  std::vector<std::thread> _client_threads;
};

} // namespace Shabby::Node

#endif // SERVER_H
