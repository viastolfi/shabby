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

class PlayerController : public Shabby::IEntityController {
public:
  ~PlayerController() = default;

  void OnInit(Shabby::Entity* entity) override;
  void OnUpdate(Shabby::Entity* entity, float dt) override;
  void OnHitboxEntered(Shabby::Hitbox* enter, Shabby::Hitbox* from) override;
  void OnHitboxExited(Shabby::Hitbox* enter, Shabby::Hitbox* from) override;
};

#endif // PLAYER_CONTROLLER_H
