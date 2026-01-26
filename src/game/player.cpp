#include "player.h"
#include "raymath.h"

namespace game {

Player::Player(const char* texture_path) 
  : Entity(texture_path) 
{}

void Player::Update(float dt) {
  Vector2 dir = {0, 0};

  if (IsKeyDown(KEY_A)) dir.x -= 1;
  if (IsKeyDown(KEY_D)) dir.x += 1;
  if (IsKeyDown(KEY_W)) dir.y -= 1;
  if (IsKeyDown(KEY_S)) dir.y += 1;

  Vector2 normalized = Vector2Normalize(dir);
  Vector2 velocity = Vector2Scale(normalized, PLAYER_SPEED);

  SetPos(GetPos() + velocity * dt);
}

} // namespace game
