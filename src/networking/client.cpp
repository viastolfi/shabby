#include "networking/client.h"

namespace engine {

Client::Client(const ClientConfig& config)
  : _config(config), _connected(false)
{
  _client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_client_socket < 0) {
    std::cerr << "[Client] Failed to create socket" << std::endl;
    return;
  }

  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(_config.port);
  
  if (inet_pton(AF_INET, _config.server_address, &server_address.sin_addr) <= 0) {
    std::cerr << "[Client] Invalid address: " << _config.server_address << std::endl;
    close(_client_socket);
    _client_socket = -1;
    return;
  }

  if (connect(_client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    std::cerr << "[Client] Failed to connect to " << _config.server_address 
              << ":" << _config.port << std::endl;
    close(_client_socket);
    _client_socket = -1;
    return;
  }
  
  int flag = 1;
  setsockopt(_client_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
  
  fcntl(_client_socket, F_SETFL, O_NONBLOCK);
  
  _connected = true;
  std::cout << "[Client] Connected to " << _config.server_address 
            << ":" << _config.port << std::endl;
}

Client::~Client()
{
  if (_client_socket >= 0) {
    close(_client_socket);
  }
}

void Client::Send(const NetworkPacket& packet)
{
  if (!_connected) return;
  
  uint8_t type_byte = static_cast<uint8_t>(packet.GetType());
  ssize_t sent = send(_client_socket, &type_byte, sizeof(type_byte), 0);
  
  if (sent <= 0) {
    std::cerr << "[Client] Failed to send packet type" << std::endl;
    _connected = false;
    return;
  }
  
  uint16_t size = static_cast<uint16_t>(packet.GetSize());
  sent = send(_client_socket, &size, sizeof(size), 0);
  
  if (sent <= 0) {
    std::cerr << "[Client] Failed to send packet size" << std::endl;
    _connected = false;
    return;
  }
  
  if (size > 0) {
    sent = send(_client_socket, packet.GetData(), size, 0);
    if (sent <= 0) {
      std::cerr << "[Client] Failed to send packet data" << std::endl;
      _connected = false;
    }
  }
}

NetworkPacket Client::Receive()
{
  if (!_connected) return NetworkPacket(PacketType::NONE);
  uint8_t type_byte;
  ssize_t bytes = recv(_client_socket, &type_byte, sizeof(type_byte), MSG_WAITALL);
  if (bytes <= 0) {
    return NetworkPacket(PacketType::NONE);
  }
  NetworkPacket packet(static_cast<PacketType>(type_byte));
  uint8_t buffer[512];
  bytes = recv(_client_socket, buffer, sizeof(buffer), MSG_DONTWAIT);
  if (bytes > 0) {
    for (ssize_t i = 0; i < bytes; i++) {
      packet.Write(buffer[i]);
    }
    packet.Reset();
  }
  return packet;
}

NetworkPacket Client::ReceiveNonBlocking()
{
  if (!_connected) return NetworkPacket(PacketType::NONE);
  uint8_t type_byte;
  ssize_t bytes = recv(_client_socket, &type_byte, sizeof(type_byte), MSG_DONTWAIT);
  if (bytes <= 0) {
    return NetworkPacket(PacketType::NONE);
  }
  NetworkPacket packet(static_cast<PacketType>(type_byte));
  
  uint16_t size;
  bytes = recv(_client_socket, &size, sizeof(size), MSG_WAITALL);
  if (bytes == sizeof(size) && size > 0) {
    uint8_t buffer[512];
    ssize_t data_bytes = recv(_client_socket, buffer, size, MSG_WAITALL);
    if (data_bytes == size) {
      for (ssize_t i = 0; i < data_bytes; i++) {
        packet.Write(buffer[i]);
      }
      packet.Reset();
    }
  }
  return packet;
}

} // namespace engine
