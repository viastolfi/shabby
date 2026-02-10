#include "actors/player_controller.h"

void PlayerController::OnInit(engine::Entity* entity)
{
  entity->_pos = {100.f, 100.f};
  entity->SetVelocity(10);
  entity->SetHitboxCreationFunction([entity]() {
    auto rOpt = entity->GetFrameRec();
    const Rectangle& r = rOpt->get();

    Hitbox* h= new RectangleHitbox(
        _id_counter++, 
        {entity->_pos.x, entity->_pos.y, 
         static_cast<float>(r.width), 
         static_cast<float>(r.height)});
    return h;
  });
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
}
