#include "entities/controllers/entity_controller.h"

namespace engine {

IEntityController::~IEntityController() {}

void IEntityController::OnHitboxEntered(Hitbox* enter, Hitbox* from)
{
  enter->_is_entered = true;
  (void)from;
}

void IEntityController::OnHitboxExited(Hitbox* enter, Hitbox* from)
{
  enter->_is_entered = false;
  (void)from;
}

} // namespace engine
