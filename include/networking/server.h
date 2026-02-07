#ifndef SERVER_H
#define SERVER_H

#include "scene/scene.h"
#include "networking/protocol/network_packet.h"
#include "networking/protocol/packet_registry.h"
#include "networking/server_logic.h"
#include "core/scheduler/scheduler.h"
#include "networking/handlers/entity_spawn_handler.h"
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <iostream>

namespace engine {

struct ServerConf {
  int port;
  int clientNumber;
  float tick_rate = 60.0f;  
};

class Server {
public:
  explicit Server(
      ServerConf conf, 
      std::unique_ptr<ServerLogic> logic,
      Scene* scene,
      Scheduler* scheduler);
  ~Server();

  void Run();
  void SendToClient(int client_socket, const NetworkPacket& packet);
  void BroadcastToAll(const NetworkPacket& packet);
  void BroadcastToAllExcept(int except_client, const NetworkPacket& packet);
  
private:
  void InitializeProtocol();
  void AcceptNewClients();
  void ProcessClientPackets();
  NetworkPacket ReceiveNonBlocking(int client_socket);
  
  int _server_socket;
  ServerConf _conf;
  std::vector<int> _connected_clients;
  std::set<int> _pending_clients;
  std::map<int, uint64_t> _client_entities;
  
  Scene* _scene;
  Scheduler* _scheduler;
  std::unique_ptr<ServerLogic> _logic;
  std::unique_ptr<PacketRegistry> _packet_registry;
};

} // namespace engine

#endif // SERVER_H
