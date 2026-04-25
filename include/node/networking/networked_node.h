#ifndef NETWORKED_NODE_H
#define NETWORKED_NODE_H

#include "node/inode.h"
#include "node/networking/network_node.h"

#include <string>
#include <memory>

namespace Shabby::Node {

// Base class for game objects that need to sync state over the network.
// Inject the shared NetworkNode via constructor (same pattern as Camera2DNode).
// Note: SyncVar uses auto-generated local IDs — assign server-side IDs for
//       robust multi-entity sync in production use.
class NetworkedNode : public INode {
public:
  NetworkedNode(std::weak_ptr<NetworkNode> net, Vector2 pos = {-1, -1});
  ~NetworkedNode() = default;

  int GetNetworkId() const { return _network_id; }
  void SyncVar(const std::string& name, const std::string& value);
  void Update(float dt) override;

protected:
  virtual void _on_network_update(const std::string& name, const std::string& value);

  std::weak_ptr<NetworkNode> _net;
  int _network_id;

private:
  static int _next_network_id;
};

} // namespace Shabby::Node

#endif // NETWORKED_NODE_H
