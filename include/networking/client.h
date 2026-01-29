#ifndef CLIENT_H
#define CLIENT_H

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "networking/packet.h"

namespace engine {

class Client {
public:
  static Client& GetInstance(int port = 8080)
  {
    static Client c(port);  

    return c;
  }

  ~Client() 
  {
    close(_client_socket); 
  }

  void Send(Packet packet)
  {
    PacketHeader header;
    header.type = static_cast<uint8_t>(packet._type);
    header.size = static_cast<uint16_t>(packet.GetSize());
    send(_client_socket, &header, sizeof(PacketHeader), 0);
    send(_client_socket, packet.GetData(), header.size, 0);
  }
  
  Packet Receive()
  {
    PacketHeader header;
    recv(_client_socket, &header, sizeof(PacketHeader), MSG_WAITALL);
    
    Packet p(static_cast<PacketType>(header.type));
    
    if (header.size > 0) {
      std::vector<uint8_t> payload(header.size);
      recv(_client_socket, payload.data(), header.size, MSG_WAITALL);
      p.Append(payload.data(), payload.size());
    }
    
    return p;
  }
  
  Packet ReceiveNonBlocking()
  {
    PacketHeader header;
    ssize_t bytes = recv(_client_socket, &header, sizeof(PacketHeader), MSG_DONTWAIT);
    
    if (bytes <= 0) {
      return Packet(PacketType::NONE);
    }
    
    Packet p(static_cast<PacketType>(header.type));
    
    if (header.size > 0) {
      std::vector<uint8_t> payload(header.size);
      recv(_client_socket, payload.data(), header.size, MSG_WAITALL);
      p.Append(payload.data(), payload.size());
    }
    
    return p;
  }
  
private:
  explicit Client(int port = 8080)
  {
    _client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    connect(
        _client_socket, 
        (struct sockaddr*)&server_address,
        sizeof(server_address));
    
    int flag = 1;
    setsockopt(_client_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
  }

  Client(const Client& c) = delete;
  void operator=(const Client& c);
  Client(Client&& c) = delete;
  Client& operator=(Client&& c) = delete;

  int _client_socket = 0;
};

} // namespace engine

#endif // CLIENT_H
