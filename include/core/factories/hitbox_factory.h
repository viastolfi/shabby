#ifndef HITBOX_FACTORY_H
#define HITBOX_FACTORY_H

#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"

namespace engine {

class HitboxFactory {
public:
  HitboxFactory() = default;

  Hitbox* CreateHitboxScaleOnTexture(Entity& entity)
  {
    const Texture2D* texture = entity.GetTexture();

    // TODO: add exception handling and only do this if texture not null

    Hitbox* h = new RectangleHitbox(
        _id_counter++, 
        {entity._pos.x, entity._pos.y, 
         static_cast<float>(texture->width), 
         static_cast<float>(texture->height)});

    // TODO: add exception handling

    return h;
  }
private:
  uint64_t _id_counter = 0;
};

} // namespace engine

#endif // HITBOX_FACTORY_H
