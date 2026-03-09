#ifndef HITBOX_FACTORY_H
#define HITBOX_FACTORY_H

#include <optional>
#include <functional>

#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"

namespace engine {

class HitboxFactory {
public:
  HitboxFactory() = default;

  Hitbox* CreateHitbox(Entity& entity)
  {
    auto f = entity.GetHitboxCreateFuncion();
    Hitbox* h = f();
    h->_id = _id_counter++;
    return h;
  }
private:
  uint64_t _id_counter = 0;
};

} // namespace engine

#endif // HITBOX_FACTORY_H
