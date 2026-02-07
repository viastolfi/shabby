#include "actors/player_controller.h"

void PlayerController::OnInit(engine::Entity* entity)
{
  entity->_pos = {200.f, 200.f};
  entity->SetVelocity(10);

  engine::NetworkedEntityController::OnInit(entity);
}

void PlayerController::OnUpdate(engine::Entity* entity, float dt) 
{
  Vector2 dir = {0, 0};

  if (IsKeyDown(KEY_A)) dir.x -= 1;
  if (IsKeyDown(KEY_D)) dir.x += 1;
  if (IsKeyDown(KEY_W)) dir.y -= 1;
  if (IsKeyDown(KEY_S)) dir.y += 1;

  Vector2 normalized = Vector2Normalize(dir);
  Vector2 velocity = Vector2Scale(normalized, 200);
  Vector2 movement = velocity * dt;

  entity->_pos = entity->_pos + movement;

  if (!Vector2Equals(dir, {0, 0}))
    engine::NetworkedEntityController::OnUpdate(entity, dt);
}
