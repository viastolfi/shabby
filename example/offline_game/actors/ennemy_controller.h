#ifndef ENNEMY_CONTROLLER_H
#define ENNEMY_CONTROLLER_H

#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"
#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"
#include "raymath.h"

class EnnemyController : public engine::IEntityController {
public:
  ~EnnemyController() = default;

  void OnInit(engine::Entity* entity) override;
  void OnUpdate(engine::Entity* entity, float dt) override;
};



#endif // ENNEMY_CONTROLLER_H
              
