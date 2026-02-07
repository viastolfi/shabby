#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H

#include <cstdint>

namespace engine {

enum class PacketType : uint8_t {
  CONNECT_REQUEST = 0,
  CONNECT_RESPONSE = 1,
  DISCONNECT = 2,
  
  ENTITY_SPAWN = 10,
  ENTITY_DESTROY = 11,
  ENTITY_UPDATE = 12,
  
  SNAPSHOT = 20,
  
  INPUT_COMMAND = 30,
  
  CUSTOM_START = 100,
  
  NONE = 255
};

} // namespace engine

#endif // PACKET_TYPE_H
