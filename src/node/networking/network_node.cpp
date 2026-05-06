#include "node/networking/network_node.h"

namespace Shabby::Node {

void NetworkNode::CreateServer(int port)
{
  _mode = NetworkMode::SERVER;
  _server = std::make_unique<Server>();
  _server->EnableTLS();

  _server->OnClientConnected = [this](int id) {
    client_connected.emit(id);
  };

  _server->OnClientDisconnected = [this](int id) {
    client_disconnected.emit(id);
  };

  _server->OnMessage = [this](int cid, const std::string& topic, const std::string& msg) {
    server_message.emit(cid, topic, msg);
  };

  _server->Start(port);
}

void NetworkNode::ConnectToServer(const std::string& ip, int port)
{
  _mode = NetworkMode::CLIENT;
  _client = std::make_unique<Client>();
  _client->EnableTLS();

  _client->OnConnected = [this]() {
    connected.emit();
  };

  _client->OnDisconnected = [this]() {
    disconnected.emit();
  };

  _client->OnMessage = [this](const std::string& topic, const std::string& msg) {
    client_message.emit(topic, msg);
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

void NetworkNode::SendToClient(int client_id, const std::string& topic, const std::string& msg)
{
  if (_mode == NetworkMode::SERVER && _server)
    _server->SendToClient(client_id, topic, msg);
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

} // namespace Shabby::Node
