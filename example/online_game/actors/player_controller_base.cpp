#include "actors/player_controller_base.h"
#include <iostream>

void PlayerControllerBase::OnInit(engine::Entity* entity)
{
  entity->_pos = {200.f, 200.f};
  entity->SetVelocity(10);
  entity->SetHitboxCreationFunction([entity]() {
    engine::Hitbox* h = new engine::RectangleHitbox(
        {entity->_pos.x, entity->_pos.y, 
         HITBOX_WIDTH, HITBOX_HEIGHT});
    return h;
  });
}

void PlayerControllerBase::OnUpdate(engine::Entity* entity, float dt)
{
  (void)entity;
  (void)dt;
}

void PlayerControllerBase::OnHitboxEntered(
    engine::Hitbox* enter, engine::Hitbox* from)
{
  Raylog::GetInstance().Log(3, "Hitbox %d entered %d", from->_id, enter->_id);
  engine::IEntityController::OnHitboxEntered(enter, from);
}

void PlayerControllerBase::OnHitboxExited(
    engine::Hitbox* enter, engine::Hitbox* from)
{
  Raylog::GetInstance().Log(3, "Hitbox %d exited %d", from->_id, enter->_id);
  engine::IEntityController::OnHitboxExited(enter, from);
}
