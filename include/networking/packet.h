#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <cstdint>

namespace engine {

enum PacketType {
  NONE = 0,

  ENTITY_SNAPSHOT,
  WORLD_SNAPSHOT,
  INPUT_COMMAND,
  
  ENTITY_CREATE_REQUEST,   
  ENTITY_CREATE_RESPONSE,
};

struct PacketHeader {
  uint8_t type;
  uint16_t size;  
};

class Packet {
public:
  PacketType _type;

  explicit Packet(PacketType type) : _type(type) {};
  ~Packet() = default;

  template<typename T>
  void Write(const T& data) 
  {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
    _buffer.insert(_buffer.end(), ptr, ptr+sizeof(T));
  }

  void Append(const void* data, size_t size)
  {
    const uint8_t* ptr = static_cast<const uint8_t*>(data);
    _buffer.insert(_buffer.end(), ptr, ptr + size);
  }

  template<typename T>
  void Read(T& data) 
  {
    data = *reinterpret_cast<T*>(&_buffer[_read_offset]); 
    _read_offset += sizeof(T);
  }

  const uint8_t* GetData() const { return _buffer.data(); }
  size_t GetSize() const { return _buffer.size(); }

private:
  std::vector<uint8_t> _buffer;
  size_t _read_offset = 0;
};

} // namespace engine

#endif // PACKET_H
