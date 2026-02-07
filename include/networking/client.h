#ifndef CLIENT_H
#define CLIENT_H

#include "networking/protocol/network_packet.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace engine {

struct ClientConfig {
  const char* server_address = "127.0.0.1";
  int port = 8080;
};

class Client {
public:
  explicit Client(const ClientConfig& config);
  ~Client();

  bool IsConnected() const { return _connected; }
  void Send(const NetworkPacket& packet);
  NetworkPacket Receive();
  NetworkPacket ReceiveNonBlocking();
  
private:
  ClientConfig _config;
  int _client_socket = -1;
  bool _connected = false;
};

} // namespace engine

#endif // CLIENT_H
