#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"
#include "raymath.h"

class PlayerController : public engine::IEntityController {
public:
  ~PlayerController() = default;

  void OnInit(engine::Entity* entity) override;
  void OnUpdate(engine::Entity* entity, float dt) override;
};

#endif // PLAYER_CONTROLLER_H
