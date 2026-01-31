#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "networking/server.h"
#include "networking/client.h"
#include "networking/server_logic.h"
#include "scene/scene.h"

namespace engine {

class NetworkManager {
public: 
  explicit NetworkManager();
  ~NetworkManager();

  void InitServer(
      ServerConf _conf, 
      std::unique_ptr<ServerLogic> logic,
      std::unique_ptr<Scene> scene);
  void RunServer();

  void InitClient();
private:
  std::unique_ptr<Server> _server;
};

} // namespace engine

#endif // NETWORK_MANAGER_H
