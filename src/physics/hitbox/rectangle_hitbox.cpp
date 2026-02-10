#include "physics/hitbox/rectangle_hitbox.h"

namespace engine {

RectangleHitbox::RectangleHitbox(uint64_t id, Rectangle shape)
  : Hitbox(id), _shape(shape)
{}

RectangleHitbox::~RectangleHitbox()
{}

} // namespace engine
