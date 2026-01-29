#ifndef SERVER_H
#define SERVER_H

#include "scene/scene.h"
#include "networking/packet.h"
#include "raylib.h"
#include "replication/snapshot/entity_snapshot.h"

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
  explicit Server(ServerConf conf);  
  ~Server();

  void Run(std::unique_ptr<Scene> scene);
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
};

} // namespace engine

#endif // SERVER_H
