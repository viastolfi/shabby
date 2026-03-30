#ifndef ENTITY_CONTROLLER_H
#define ENTITY_CONTROLLER_H

#include "physics/hitbox/hitbox.h"

namespace Shabby {

class Entity;

class IEntityController {
public:
  virtual ~IEntityController() = 0;
  virtual void OnInit(Entity* entity) = 0;
  virtual void OnUpdate(Entity* entity, float dt) = 0;
  virtual void OnHitboxEntered(Hitbox* enter, Hitbox* from);
  virtual void OnHitboxExited(Hitbox* enter, Hitbox* from);
};

} // namespace Shabby

#endif // ENTITY_CONTROLLER_H

