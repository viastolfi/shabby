#include "networking/server.h"

namespace engine {

Server::Server(ServerConf conf)
  :_conf(conf)
{
  _server_socket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(_conf.port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  bind(
      _server_socket, 
      (struct sockaddr*)&server_address,
      sizeof(server_address));

  listen(_server_socket, _conf.clientNumber);
}

Server::~Server()
{
  close(_server_socket);
}

size_t Server::GenerateEntityId()
{
  return _next_entity_id++;
}

void Server::BroadcastSnapshot(const std::vector<int>& clients, Packet snapshot)
{
  for (int client_socket : clients) {
    Send(client_socket, snapshot);
  }
}

void Server::Run(std::unique_ptr<Scene> scene) 
{
  std::cout << "Server starting on port " << _conf.port << "..." << std::endl;
  
  fcntl(_server_socket, F_SETFL, O_NONBLOCK);
  
  auto last_tick = std::chrono::high_resolution_clock::now();
  const float tick_rate = 1.0f / 60.0f; 
  
  while (true) {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - last_tick).count();
    
    int new_client = accept(_server_socket, nullptr, nullptr);
    if (new_client >= 0) {
      int flag = 1;
      setsockopt(new_client, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
      
      _connected_clients.push_back(new_client);
      std::cout << "New client connected: " << new_client << std::endl;
    }
    
    for (int client_socket : _connected_clients) {
      Packet packet = ReceiveNonBlocking(client_socket);
      if (packet._type != PacketType::NONE) {
        HandlePacket(client_socket, packet, scene.get());
      }
    }
    
    if (dt >= tick_rate) {
      last_tick = now;
      scene->UpdateScene(dt);
      
      if (!_connected_clients.empty()) {
        Packet snapshot = scene->GenerateWorldSnapshot();
        BroadcastSnapshot(_connected_clients, snapshot);
      }
    }
    
    usleep(5000); 
  }
}

void Server::HandlePacket(int client_socket, Packet& packet, Scene* scene)
{
  switch (packet._type) {
    case PacketType::ENTITY_CREATE_REQUEST: {
      Vector2 position;
      int texture_id;
      packet.Read(position);
      packet.Read(texture_id);
      
      size_t entity_id = GenerateEntityId();
      
      scene->AddEntity(std::make_unique<Entity>(
            std::make_unique<Sprite>(texture_id),
            position, 
            entity_id));
      
      Packet response(PacketType::ENTITY_CREATE_RESPONSE);
      response.Write(entity_id);
      response.Write(texture_id);
      Send(client_socket, response);
      
      std::cout << "Entity created with ID: " << entity_id << " at position { " 
                << position.x << ":" << position.y << " }" 
                << " with texture id: " << texture_id << std::endl;
      break;
    }
    
    case PacketType::INPUT_COMMAND: {
      size_t id;
      Vector2 direction;
      int texture_id;

      packet.Read(id);
      packet.Read(direction);
      packet.Read(texture_id);

      EntitySnapshot snapshot(id, direction, texture_id);
      scene->ApplySnapshot(snapshot); 
      break;
    }

    case PacketType::NONE: 
    case PacketType::ENTITY_SNAPSHOT:
    case PacketType::WORLD_SNAPSHOT:
      break;

    default:
      break; 
  }
}

void Server::Send(int client_socket, Packet packet)
{
  PacketHeader header;
  header.type = static_cast<uint8_t>(packet._type);
  header.size = static_cast<uint16_t>(packet.GetSize());
  send(client_socket, &header, sizeof(PacketHeader), 0);
  send(client_socket, packet.GetData(), header.size, 0);
}

Packet Server::Receive(int client_socket) 
{
  PacketHeader header;

  recv(client_socket, &header, sizeof(PacketHeader), MSG_WAITALL);

  Packet p(static_cast<PacketType>(header.type));

  std::vector<uint8_t> payload(header.size);
  recv(client_socket, payload.data(), header.size, MSG_WAITALL); 

  p.Append(payload.data(), payload.size());

  return p;
}

Packet Server::ReceiveNonBlocking(int client_socket)
{
  PacketHeader header;
  
  ssize_t bytes = recv(client_socket, &header, sizeof(PacketHeader), MSG_DONTWAIT);
  if (bytes <= 0) {
    return Packet(PacketType::NONE);
  }
  
  Packet p(static_cast<PacketType>(header.type));
  
  std::vector<uint8_t> payload(header.size);
  recv(client_socket, payload.data(), header.size, MSG_WAITALL);
  
  p.Append(payload.data(), payload.size());
  
  return p;
}

} // namespace engine
