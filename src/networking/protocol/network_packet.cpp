#include "networking/protocol/network_packet.h"

namespace engine {

void NetworkPacket::EnsureCapacity(size_t additional) {
  if (_write_pos + additional > _data.size()) {
    _data.resize(std::max(_data.size() * 2, _write_pos + additional));
  }
}

void NetworkPacket::Write(uint8_t value) {
  EnsureCapacity(sizeof(value));
  _data[_write_pos++] = value;
}

void NetworkPacket::Write(uint16_t value) {
  EnsureCapacity(sizeof(value));
  std::memcpy(&_data[_write_pos], &value, sizeof(value));
  _write_pos += sizeof(value);
}

void NetworkPacket::Write(uint32_t value) {
  EnsureCapacity(sizeof(value));
  std::memcpy(&_data[_write_pos], &value, sizeof(value));
  _write_pos += sizeof(value);
}

void NetworkPacket::Write(uint64_t value) {
  EnsureCapacity(sizeof(value));
  std::memcpy(&_data[_write_pos], &value, sizeof(value));
  _write_pos += sizeof(value);
}

void NetworkPacket::Write(float value) {
  EnsureCapacity(sizeof(value));
  std::memcpy(&_data[_write_pos], &value, sizeof(value));
  _write_pos += sizeof(value);
}

void NetworkPacket::Write(const Vector2& vec) {
  Write(vec.x);
  Write(vec.y);
}

void NetworkPacket::Write(const char* str) {
  uint16_t len = std::strlen(str);
  Write(len);
  EnsureCapacity(len);
  std::memcpy(&_data[_write_pos], str, len);
  _write_pos += len;
}

void NetworkPacket::Read(uint8_t& value) {
  if (_read_pos + sizeof(value) <= _write_pos) {
    value = _data[_read_pos++];
  }
}

void NetworkPacket::Read(uint16_t& value) {
  if (_read_pos + sizeof(value) <= _write_pos) {
    std::memcpy(&value, &_data[_read_pos], sizeof(value));
    _read_pos += sizeof(value);
  }
}

void NetworkPacket::Read(uint32_t& value) {
  if (_read_pos + sizeof(value) <= _write_pos) {
    std::memcpy(&value, &_data[_read_pos], sizeof(value));
    _read_pos += sizeof(value);
  }
}

void NetworkPacket::Read(uint64_t& value) {
  if (_read_pos + sizeof(value) <= _write_pos) {
    std::memcpy(&value, &_data[_read_pos], sizeof(value));
    _read_pos += sizeof(value);
  }
}

void NetworkPacket::Read(float& value) {
  if (_read_pos + sizeof(value) <= _write_pos) {
    std::memcpy(&value, &_data[_read_pos], sizeof(value));
    _read_pos += sizeof(value);
  }
}

void NetworkPacket::Read(Vector2& vec) {
  Read(vec.x);
  Read(vec.y);
}

} // namespace engine
