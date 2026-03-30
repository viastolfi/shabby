#ifndef PLAYER_CONTROLLER_BASE_H
#define PLAYER_CONTROLLER_BASE_H

#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"
#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"

class PlayerControllerBase : public Shabby::IEntityController {
public:
  ~PlayerControllerBase() override = default;

  void OnInit(Shabby::Entity* entity) override;
  void OnUpdate(Shabby::Entity* entity, float dt) override;
  void OnHitboxEntered(Shabby::Hitbox* enter, Shabby::Hitbox* from) override;
  void OnHitboxExited(Shabby::Hitbox* enter, Shabby::Hitbox* from) override;

protected:
  static constexpr float HITBOX_WIDTH = 16.f;
  static constexpr float HITBOX_HEIGHT = 16.f;
};

#endif // PLAYER_CONTROLLER_BASE_H
