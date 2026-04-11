#ifndef NETWORK_NODE_H
#define NETWORK_NODE_H

#include "node/inode.h"
#include "node/networking/server.h"
#include "node/networking/client.h"

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

  void Update(float dt) override;

  bool ShouldClose();
  NetworkMode GetMode() const;

protected:
  virtual void _on_receive_message(const std::string& topic, const std::string& message);
  virtual void _on_receive_message(int client_id, const std::string& topic, const std::string& message);

  virtual void _on_client_connected(int client_id);
  virtual void _on_client_disconnected(int client_id);

  virtual void _on_connected();
  virtual void _on_disconnected();

  Server& GetServer();
  Client& GetClient();

private:
  NetworkMode _mode = NetworkMode::NONE;
  std::unique_ptr<Server> _server;
  std::unique_ptr<Client> _client;
};

} // namespace Shabby::Node

#endif // NETWORK_NODE_H
