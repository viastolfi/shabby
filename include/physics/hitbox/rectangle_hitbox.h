#ifndef RECTANGLE_HITBOX_h
#define RECTANGLE_HITBOX_h

#include <cstdint>
#include "raylib.h"
#include "physics/hitbox/hitbox.h"

namespace engine {

class RectangleHitbox : public Hitbox {
public:
  explicit RectangleHitbox(Rectangle shape); 
  ~RectangleHitbox();

  void Draw() override;
  void Update(Vector2 new_pos) override;
private:
  Rectangle _shape;
}; 

} // namespace engine

#endif // RECTANGLE_HITBOX_h
