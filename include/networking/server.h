#ifndef SERVER_H
#define SERVER_H

#include "scene/scene.h"
#include "networking/packet.h"
#include "raylib.h"
#include "replication/snapshot/entity_snapshot.h"
#include "core/scheduler/scheduler.h"
#include "networking/server_logic.h"
#include "core/game_simulation/game_simulation.h"

#include <memory>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <chrono>
#include <fcntl.h>

namespace engine {

struct ServerConf {
  int port;
  int clientNumber;
};

class Server {
public:
  explicit Server(
      ServerConf conf, 
      std::unique_ptr<ServerLogic> logic,
      std::unique_ptr<Scene> scene);
  ~Server();

  void Run();
  Packet Receive(int client_socket);
  Packet ReceiveNonBlocking(int client_socket);
  void Send(int client_socket, Packet packet);
  void BroadcastSnapshot(const std::vector<int>& clients, Packet snapshot);
  void HandlePacket(int client_socket, Packet& packet, Scene* scene);
  
  size_t GenerateEntityId();
private:
  int _server_socket = 0;
  ServerConf _conf;
  size_t _next_entity_id = 1;
  std::vector<int> _connected_clients;
  std::unique_ptr<ServerLogic> _logic;
  std::unique_ptr<GameSimulation> _game_simulation;
};

} // namespace engine

#endif // SERVER_H
