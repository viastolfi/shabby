#ifndef NETWORKED_NODE_H
#define NETWORKED_NODE_H

#include "node/inode.h"
#include "node/networking/network_node.h"

#include <string>
#include <memory>

namespace Shabby::Node {

class NetworkedNode : public INode {
public:
  NetworkedNode() = default;
  NetworkedNode(Vector2 pos);
  ~NetworkedNode() = default;

  void SetNetworkNode(std::shared_ptr<NetworkNode> net);

  void SyncVar(const std::string& name, const std::string& value);
  void Update(float dt) override;

protected:
  virtual void _on_network_update(const std::string& name, const std::string& value);

  std::shared_ptr<NetworkNode> _network_node;
  int _network_id = -1;

private:
  static int _next_network_id;
};

} // namespace Shabby::Node

#endif // NETWORKED_NODE_H
