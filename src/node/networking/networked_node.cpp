#include "node/networking/networked_node.h"

namespace Shabby::Node {

int NetworkedNode::_next_network_id = 0;

NetworkedNode::NetworkedNode(Vector2 pos)
  :INode(pos)
{
  _network_id = _next_network_id++;
}

void NetworkedNode::SetNetworkNode(std::shared_ptr<NetworkNode> net)
{
  _network_node = net;
}

void NetworkedNode::SyncVar(const std::string& name, const std::string& value)
{
  if (!_network_node)
    return;

  std::string topic = "sync:" + std::to_string(_network_id) + ":" + name;

  if (_network_node->GetMode() == NetworkMode::SERVER)
    _network_node->Broadcast(topic, value);
  else if (_network_node->GetMode() == NetworkMode::CLIENT)
    _network_node->Send(topic, value);
}

void NetworkedNode::Update(float dt)
{
  INode::Update(dt);
}

void NetworkedNode::_on_network_update(const std::string& name, const std::string& value)
{
  (void)name;
  (void)value;
}

} // namespace Shabby::Node
