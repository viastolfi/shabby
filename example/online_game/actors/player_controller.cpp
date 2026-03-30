#include "actors/player_controller.h"

PlayerController::PlayerController(engine::Client* client)
  : _network(std::make_unique<engine::NetworkedEntityController>(client))
{}

void PlayerController::OnInit(engine::Entity* entity)
{
  PlayerControllerBase::OnInit(entity);
  _network->OnInit(entity);
}

void PlayerController::OnUpdate(engine::Entity* entity, float dt) 
{
  Vector2 dir = {0, 0};

  if (IsKeyDown(KEY_A)) dir.x -= 1;
  if (IsKeyDown(KEY_D)) dir.x += 1;
  if (IsKeyDown(KEY_W)) dir.y -= 1;
  if (IsKeyDown(KEY_S)) dir.y += 1;

  if (dir.x == 0 && dir.y == 0) {
    entity->PlayAnimation(0);
  } else if (dir.y > 0) {
    entity->PlayAnimation(1);
  } else if (dir.y < 0) {
    entity->PlayAnimation(2);
  } else if (dir.x < 0) {
    entity->PlayAnimation(3);
  } else if (dir.x > 0) {
    entity->PlayAnimation(4);
  }

  Vector2 normalized = Vector2Normalize(dir);
  Vector2 velocity = Vector2Scale(normalized, 200);
  Vector2 movement = velocity * dt;

  entity->_pos = entity->_pos + movement;

  if (!Vector2Equals(dir, {0, 0}))
    _network->OnUpdate(entity, dt);
}
