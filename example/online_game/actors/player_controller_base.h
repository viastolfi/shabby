#ifndef PLAYER_CONTROLLER_BASE_H
#define PLAYER_CONTROLLER_BASE_H

#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"
#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"

class PlayerControllerBase : public engine::IEntityController {
public:
  ~PlayerControllerBase() override = default;

  void OnInit(engine::Entity* entity) override;
  void OnUpdate(engine::Entity* entity, float dt) override;
  void OnHitboxEntered(engine::Hitbox* enter, engine::Hitbox* from) override;
  void OnHitboxExited(engine::Hitbox* enter, engine::Hitbox* from) override;

protected:
  static constexpr float HITBOX_WIDTH = 16.f;
  static constexpr float HITBOX_HEIGHT = 16.f;
};

#endif // PLAYER_CONTROLLER_BASE_H
