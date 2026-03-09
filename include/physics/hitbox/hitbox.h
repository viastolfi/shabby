#ifndef HITBOX_H
#define HITBOX_H

#include <cstdint>
#include "raylib.h"

namespace engine {

class Hitbox {
public:
  explicit Hitbox() = default;
  virtual ~Hitbox() = 0;

  virtual void Draw() = 0;
  virtual void Update(Vector2 new_pos) = 0;

  virtual const Rectangle GetShape() const = 0;
  uint64_t _id;

  bool _is_entered = false;
};

} // namespace engine

#endif // HITBOX_H
