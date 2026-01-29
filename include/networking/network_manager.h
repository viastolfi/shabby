#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "networking/server.h"
#include "networking/client.h"
#include "scene/scene.h"

namespace engine {

class NetworkManager {
public: 
  explicit NetworkManager();
  ~NetworkManager();

  void InitServer(ServerConf _conf);
  void RunServer(std::unique_ptr<Scene> scene);

  void InitClient();
private:
  std::unique_ptr<Server> _server;
};

} // namespace engine

#endif // NETWORK_MANAGER_H
