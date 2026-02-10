#ifndef HITBOX_H
#define HITBOX_H

#include <cstdint>

namespace engine {

class Hitbox {
public:
  explicit Hitbox(uint64_t id) 
    : _id(id)
  {}
  virtual ~Hitbox() = 0;
private:
  uint64_t _id;
};

} // namespace engine

#endif // HITBOX_H
