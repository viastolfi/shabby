#include "actors/ennemy_controller.h"

void EnnemyController::OnInit(engine::Entity* entity)
{
  entity->_pos = {100.f, 100.f};
  entity->SetVelocity(10);
  entity->SetHitboxCreationFunction([entity]() {
    auto r = entity->GetSpriteRectangle();

    engine::Hitbox* h = new engine::RectangleHitbox(
        {entity->_pos.x, entity->_pos.y, 
         static_cast<float>(r.width), 
         static_cast<float>(r.height)});
    return h;
  });

   
}

void EnnemyController::OnUpdate(engine::Entity* entity, float dt)
{
  (void)(entity);
  (void)(dt);
}
