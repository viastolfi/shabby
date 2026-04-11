#include "node/networking/network_node.h"

namespace Shabby::Node {

void NetworkNode::CreateServer(int port)
{
  _mode = NetworkMode::SERVER;
  _server = std::make_unique<Server>();

  _server->OnClientConnected = [this](int client_id) {
    _on_client_connected(client_id);
  };

  _server->OnClientDisconnected = [this](int client_id) {
    _on_client_disconnected(client_id);
  };

  _server->OnMessage = [this](int client_id, const std::string& topic, const std::string& message) {
    _on_receive_message(client_id, topic, message);
  };

  _server->Start(port);
}

void NetworkNode::ConnectToServer(const std::string& ip, int port)
{
  _mode = NetworkMode::CLIENT;
  _client = std::make_unique<Client>();

  _client->OnConnected = [this]() {
    _on_connected();
  };

  _client->OnDisconnected = [this]() {
    _on_disconnected();
  };

  _client->OnMessage = [this](const std::string& topic, const std::string& message) {
    _on_receive_message(topic, message);
  };

  _client->Connect(ip, port);
}

void NetworkNode::Send(const std::string& topic, const std::string& msg)
{
  if (_mode == NetworkMode::CLIENT && _client)
    _client->Send(topic, msg);
}

void NetworkNode::Broadcast(const std::string& topic, const std::string& msg)
{
  if (_mode == NetworkMode::SERVER && _server)
    _server->Broadcast(topic, msg);
}

void NetworkNode::Update(float dt)
{
  if (_mode == NetworkMode::SERVER && _server)
    _server->Poll();
  else if (_mode == NetworkMode::CLIENT && _client)
    _client->Poll();

  INode::Update(dt);
}

bool NetworkNode::ShouldClose()
{
  return false;
}

NetworkMode NetworkNode::GetMode() const
{
  return _mode;
}

void NetworkNode::_on_receive_message(const std::string& topic, const std::string& message)
{
  (void)topic;
  (void)message;
}

void NetworkNode::_on_receive_message(int client_id, const std::string& topic, const std::string& message)
{
  (void)client_id;
  (void)topic;
  (void)message;
}

void NetworkNode::_on_client_connected(int client_id)
{
  (void)client_id;
}

void NetworkNode::_on_client_disconnected(int client_id)
{
  (void)client_id;
}

void NetworkNode::_on_connected() {}

void NetworkNode::_on_disconnected() {}

Server& NetworkNode::GetServer()
{
  return *_server;
}

Client& NetworkNode::GetClient()
{
  return *_client;
}

} // namespace Shabby::Node
