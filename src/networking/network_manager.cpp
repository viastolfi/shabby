#include "networking/network_manager.h"

namespace engine {

NetworkManager::NetworkManager()
{}

NetworkManager::~NetworkManager()
{}

void NetworkManager::InitServer(
    ServerConf conf, 
    std::unique_ptr<ServerLogic> logic,
    std::unique_ptr<Scene> scene)
{
  _server = std::make_unique<Server>(
      conf, std::move(logic), std::move(scene));
}

void NetworkManager::RunServer() 
{
  if (_server)
    _server->Run();
}

void NetworkManager::InitClient() 
{
  Client::GetInstance();
}

} // namespace engine
