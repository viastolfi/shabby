#include "actors/player_controller_base.h"
#include <iostream>

void PlayerControllerBase::OnInit(Shabby::Entity* entity)
{
  entity->_pos = {200.f, 200.f};
  entity->SetVelocity(10);
  entity->SetHitboxCreationFunction([entity]() {
    Shabby::Hitbox* h = new Shabby::RectangleHitbox(
        {entity->_pos.x, entity->_pos.y, 
         HITBOX_WIDTH, HITBOX_HEIGHT});
    return h;
  });
}

void PlayerControllerBase::OnUpdate(Shabby::Entity* entity, float dt)
{
  (void)entity;
  (void)dt;
}

void PlayerControllerBase::OnHitboxEntered(
    Shabby::Hitbox* enter, Shabby::Hitbox* from)
{
  Raylog::GetInstance().Log(3, "Hitbox %d entered %d", from->_id, enter->_id);
  Shabby::IEntityController::OnHitboxEntered(enter, from);
}

void PlayerControllerBase::OnHitboxExited(
    Shabby::Hitbox* enter, Shabby::Hitbox* from)
{
  Raylog::GetInstance().Log(3, "Hitbox %d exited %d", from->_id, enter->_id);
  Shabby::IEntityController::OnHitboxExited(enter, from);
}
