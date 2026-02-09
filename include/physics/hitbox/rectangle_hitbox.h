#ifndef RECTANGLE_HITBOX_h
#define RECTANGLE_HITBOX_h

#include "raylib.h"
#include "physics/hitbox/hitbox.h"

namespace engine {

class RectangleHitbox {
public:
  explicit RectangleHitbox() = default;
  ~RectangleHitbox() = default;
private:
  Rectangle _shape;
}; 

} // namespace engine

#endif // RECTANGLE_HITBOX_h
