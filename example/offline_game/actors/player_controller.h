#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"
#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"
#include "utils/raylog.h"
#include "raymath.h"
#include "raylib.h"
#include "asset_ids.h"

class PlayerController : public engine::IEntityController {
public:
  ~PlayerController() = default;

  void OnInit(engine::Entity* entity) override;
  void OnUpdate(engine::Entity* entity, float dt) override;
  void OnHitboxEntered(engine::Hitbox* enter, engine::Hitbox* from) override;
  void OnHitboxExited(engine::Hitbox* enter, engine::Hitbox* from) override;
};

#endif // PLAYER_CONTROLLER_H
