#include "networking/server.h"

namespace engine {

Server::Server(
    ServerConf conf,
    std::unique_ptr<ServerLogic> logic,
    Scene* scene,
    Scheduler* scheduler)
  : _conf(conf),
    _scene(scene),
    _scheduler(scheduler),
    _logic(std::move(logic)),
    _packet_registry(std::make_unique<PacketRegistry>())
{
  _server_socket = socket(AF_INET, SOCK_STREAM, 0);
  
  int reuse = 1;
  setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  
  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(_conf.port);
  server_address.sin_addr.s_addr = INADDR_ANY;
  
  bind(_server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
  listen(_server_socket, _conf.clientNumber);
  
  fcntl(_server_socket, F_SETFL, O_NONBLOCK);
  
  InitializeProtocol();
  
  _logic->RegisterCustomPackets(_packet_registry.get());
  
  _logic->OnStart(_scene, _scheduler, this);
  
  std::cout << "[Server] Started on port " << _conf.port << std::endl;
}

Server::~Server()
{
  close(_server_socket);
}

void Server::InitializeProtocol()
{
  _packet_registry->RegisterHandler(
    PacketType::ENTITY_SPAWN,
    std::make_unique<EntitySpawnHandler>()
  );
  _packet_registry->RegisterHandler(
    PacketType::INPUT_COMMAND,
    std::make_unique<InputCommandHandler>()
  );
}

void Server::Run()
{
  auto last_tick = std::chrono::high_resolution_clock::now();
  auto last_snapshot = std::chrono::high_resolution_clock::now();
  const float tick_interval = 1.0f / _conf.tick_rate;
  const float snapshot_interval = 1.0f / 60.0f;
  
  while (true) {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - last_tick).count();
    float snapshot_dt = std::chrono::duration<float>(now - last_snapshot).count();
    
    AcceptNewClients();
    
    ProcessClientPackets();
    
    if (dt >= tick_interval) {
      last_tick = now;
      _logic->OnUpdate(_scene, dt);
      _scene->Update(dt);
    }
    
    if (snapshot_dt >= snapshot_interval) {
      last_snapshot = now;
      WorldSnapshot snapshot = _scene->CreateWorldSnapshot();
      NetworkPacket packet = snapshot.SnapshotToPacket();
      BroadcastToAll(packet);
    }
    
    usleep(1000); 
  }
}

void Server::AcceptNewClients()
{
  int new_client = accept(_server_socket, nullptr, nullptr);
  if (new_client >= 0) {
    int flag = 1;
    setsockopt(new_client, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
    
    _connected_clients.push_back(new_client);
    _pending_clients.insert(new_client);
    _logic->OnClientConnect(new_client, _scene);
  }
}

void Server::ProcessClientPackets()
{
  for (auto it = _connected_clients.begin(); it != _connected_clients.end(); ) {
    int client_socket = *it;
    NetworkPacket packet = ReceiveNonBlocking(client_socket);
    
    if (packet.GetType() == PacketType::DISCONNECT) {
      std::cout << "[Server] Client " << client_socket << " disconnected" << std::endl;
      
      if (_client_entities.count(client_socket) > 0) {
        uint64_t entity_id = _client_entities[client_socket];
        _scene->RemoveEntity(entity_id);
        
        NetworkPacket destroy_packet(PacketType::ENTITY_DESTROY);
        destroy_packet.Write(entity_id);
        BroadcastToAllExcept(client_socket, destroy_packet);
        
        _client_entities.erase(client_socket);
      }
      
      close(client_socket);
      _pending_clients.erase(client_socket);
      _logic->OnClientDisconnect(client_socket, _scene);
      it = _connected_clients.erase(it);
      continue;
    }
    
    if (packet.GetType() != PacketType::NONE) {
      NetworkPacket* response = 
        _packet_registry->RouteFromClient(client_socket, packet, _scene);
      if (response != nullptr) {
        if (response->GetType() == PacketType::ENTITY_SPAWN) {
          response->Reset();
          uint64_t entity_id;
          Vector2 position;
          uint32_t texture_id;
          response->Read(entity_id);
          response->Read(position);
          response->Read(texture_id);
          
          _client_entities[client_socket] = entity_id;
          
          NetworkPacket spawn_response(PacketType::ENTITY_SPAWN);
          spawn_response.Write(entity_id);
          spawn_response.Write(position);
          spawn_response.Write(texture_id);
          
          SendToClient(client_socket, spawn_response);
          
          if (_pending_clients.count(client_socket) > 0) {
            _pending_clients.erase(client_socket);
            for (const auto* entity : _scene->GetEntities()) {
              if (entity->_id != entity_id) {
                NetworkPacket spawn_packet(PacketType::ENTITY_SPAWN);
                spawn_packet.Write(static_cast<uint64_t>(entity->_id));
                spawn_packet.Write(entity->_pos);
                spawn_packet.Write(static_cast<uint32_t>(
                      entity->GetSpriteTextureId()));
                SendToClient(client_socket, spawn_packet);
              }
            }
          }
          
          BroadcastToAllExcept(client_socket, spawn_response);
          delete response;
        }
      }
    }
    ++it;
  }
}

NetworkPacket Server::ReceiveNonBlocking(int client_socket)
{
  NetworkPacket packet;
  uint8_t type_byte;
  ssize_t bytes = recv(client_socket, &type_byte, sizeof(type_byte), MSG_DONTWAIT);
  
  if (bytes == 0) {
    packet.SetType(PacketType::DISCONNECT);
    return packet;
  }
  
  if (bytes < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return packet;
    } else {
      packet.SetType(PacketType::DISCONNECT);
      return packet;
    }
  }
  
  packet.SetType(static_cast<PacketType>(type_byte));
  
  uint16_t size;
  bytes = recv(client_socket, &size, sizeof(size), MSG_WAITALL);
  if (bytes != sizeof(size)) {
    packet.SetType(PacketType::DISCONNECT);
    return packet;
  }
  
  if (size > 0) {
    uint8_t buffer[512];
    ssize_t data_bytes = recv(client_socket, buffer, size, MSG_WAITALL);
    if (data_bytes != size) {
      packet.SetType(PacketType::DISCONNECT);
      return packet;
    }
    for (ssize_t i = 0; i < data_bytes; i++) {
      packet.Write(buffer[i]);
    }
    packet.Reset();
  }
  
  return packet;
}

void Server::SendToClient(int client_socket, const NetworkPacket& packet)
{
  uint8_t type_byte = static_cast<uint8_t>(packet.GetType());
  send(client_socket, &type_byte, sizeof(type_byte), 0);
  
  uint16_t size = static_cast<uint16_t>(packet.GetSize());
  send(client_socket, &size, sizeof(size), 0);
  
  if (size > 0) {
    send(client_socket, packet.GetData(), size, 0);
  }
}

void Server::BroadcastToAll(const NetworkPacket& packet)
{
  for (int client : _connected_clients) {
    SendToClient(client, packet);
  }
}

void Server::BroadcastToAllExcept(int except_client, const NetworkPacket& packet)
{
  for (int client : _connected_clients) {
    if (client != except_client) {
      SendToClient(client, packet);
    }
  }
}

} // namespace engine
