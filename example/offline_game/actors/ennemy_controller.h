#ifndef ENNEMY_CONTROLLER_H
#define ENNEMY_CONTROLLER_H

#include "entities/controllers/entity_controller.h"
#include "entities/entity.h"
#include "physics/hitbox/hitbox.h"
#include "physics/hitbox/rectangle_hitbox.h"
#include "raymath.h"

class EnnemyController : public Shabby::IEntityController {
public:
  ~EnnemyController() = default;

  void OnInit(Shabby::Entity* entity) override;
  void OnUpdate(Shabby::Entity* entity, float dt) override;
};



#endif // ENNEMY_CONTROLLER_H
              
