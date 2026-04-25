#ifndef NETWORK_NODE_H
#define NETWORK_NODE_H

#include "node/inode.h"
#include "node/networking/server.h"
#include "node/networking/client.h"
#include "core/signal/signal.h"

#include <memory>
#include <string>

namespace Shabby::Node {

enum class NetworkMode {
  NONE,
  SERVER,
  CLIENT
};

class NetworkNode : public INode {
public:
  NetworkNode() = default;
  ~NetworkNode() = default;

  void CreateServer(int port);
  void ConnectToServer(const std::string& ip, int port);

  void Send(const std::string& topic, const std::string& msg);
  void Broadcast(const std::string& topic, const std::string& msg);
  void SendToClient(int client_id, const std::string& topic, const std::string& msg);

  void Update(float dt) override;

  bool ShouldClose();
  NetworkMode GetMode() const;

  // Server-side signals
  Core::Signal<int>                           client_connected;
  Core::Signal<int>                           client_disconnected;
  Core::Signal<int, std::string, std::string> server_message;

  // Client-side signals
  Core::Signal<>                              connected;
  Core::Signal<>                              disconnected;
  Core::Signal<std::string, std::string>      client_message;

private:
  NetworkMode _mode = NetworkMode::NONE;
  std::unique_ptr<Server> _server;
  std::unique_ptr<Client> _client;
};

} // namespace Shabby::Node

#endif // NETWORK_NODE_H
