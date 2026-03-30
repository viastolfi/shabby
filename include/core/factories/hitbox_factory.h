#ifndef HITBOX_FACTORY_H
#define HITBOX_FACTORY_H

#include <optional>
#include <functional>

#include "raylib.h"
#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"

namespace Shabby {

class HitboxFactory {
public:
  HitboxFactory() = default;

  Hitbox* CreateHitbox(Entity& entity)
  {
    auto f = entity.GetHitboxCreateFunction();
    Hitbox* h = f();
    h->_id = _id_counter++;
    return h;
  }

  Hitbox* CreateBaseRectangleHitbox(
      float x, float y, float width, float height) 
  {
    Hitbox* h = new RectangleHitbox({x, y, width, height});
    
    // TODO: add memory safety

    h->_id = _id_counter++;

    return h;
  }

private:
  uint64_t _id_counter = 0;
};

} // namespace Shabby

#endif // HITBOX_FACTORY_H
