#ifndef NETWORK_PACKET_H
#define NETWORK_PACKET_H

#include "networking/protocol/packet_type.h"
#include "utils/vector2.h"
#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>

namespace engine {

class NetworkPacket {
public:
  NetworkPacket() : _type(PacketType::NONE), _write_pos(0), _read_pos(0) {
    _data.resize(1024); 
  }
  
  explicit NetworkPacket(PacketType type) : _type(type), _write_pos(0), _read_pos(0) {
    _data.resize(1024);
  }
  
  PacketType GetType() const { return _type; }
  void SetType(PacketType type) { _type = type; }
  
  const uint8_t* GetData() const { return _data.data(); }
  size_t GetSize() const { return _write_pos; }
  
  void Write(uint8_t value);
  void Write(uint16_t value);
  void Write(uint32_t value);
  void Write(uint64_t value);
  void Write(float value);
  void Write(const Vector2& vec);
  void Write(const char* str);
  
  void Read(uint8_t& value);
  void Read(uint16_t& value);
  void Read(uint32_t& value);
  void Read(uint64_t& value);
  void Read(float& value);
  void Read(Vector2& vec);
  
  void Reset() { _read_pos = 0; }
  
private:
  PacketType _type;
  std::vector<uint8_t> _data;
  size_t _write_pos;
  size_t _read_pos;
  
  void EnsureCapacity(size_t additional);
};

} // namespace engine

#endif // NETWORK_PACKET_H
