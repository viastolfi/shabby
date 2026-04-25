#include "node/networking/networked_node.h"

namespace Shabby::Node {

int NetworkedNode::_next_network_id = 0;

NetworkedNode::NetworkedNode(std::weak_ptr<NetworkNode> net, Vector2 pos)
  : INode(pos), _net(net), _network_id(_next_network_id++)
{
  auto node = _net.lock();
  if (!node) return;

  std::string prefix = "sync:" + std::to_string(_network_id) + ":";

  // Subscribe to both signals — only one mode is ever active per run.
  node->server_message.connect([this, prefix](int, const std::string& topic, const std::string& value) {
    if (topic.rfind(prefix, 0) == 0)
      _on_network_update(topic.substr(prefix.size()), value);
  });

  node->client_message.connect([this, prefix](const std::string& topic, const std::string& value) {
    if (topic.rfind(prefix, 0) == 0)
      _on_network_update(topic.substr(prefix.size()), value);
  });
}

void NetworkedNode::SyncVar(const std::string& name, const std::string& value)
{
  auto node = _net.lock();
  if (!node) return;

  std::string topic = "sync:" + std::to_string(_network_id) + ":" + name;

  if (node->GetMode() == NetworkMode::SERVER)
    node->Broadcast(topic, value);
  else if (node->GetMode() == NetworkMode::CLIENT)
    node->Send(topic, value);
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
