#include "networking/network_manager.h"

namespace engine {

NetworkManager::NetworkManager()
{}

NetworkManager::~NetworkManager()
{}

void NetworkManager::InitServer(ServerConf conf)
{
  _server = std::make_unique<Server>(conf);
}

void NetworkManager::RunServer(std::unique_ptr<Scene> scene) 
{
  if (_server)
    _server->Run(std::move(scene));
}

void NetworkManager::InitClient() 
{
  Client::GetInstance();
}

} // namespace engine
