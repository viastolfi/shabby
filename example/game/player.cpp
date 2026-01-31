#include "player.h"

namespace game {

Player::Player(std::unique_ptr<engine::Sprite> s) 
  : Entity(std::move(s)) 
{}

Player::Player(std::unique_ptr<engine::Sprite> s, Vector2 pos)
  : Entity(std::move(s), pos)
{}

Player::Player(Vector2 pos, size_t id)
  : Entity(pos, id)
{}

void Player::Update(float dt) {
  Vector2 dir = {0, 0};

  if (IsKeyDown(KEY_A)) dir.x -= 1;
  if (IsKeyDown(KEY_D)) dir.x += 1;
  if (IsKeyDown(KEY_W)) dir.y -= 1;
  if (IsKeyDown(KEY_S)) dir.y += 1;

  Vector2 normalized = Vector2Normalize(dir);
  Vector2 velocity = Vector2Scale(normalized, PLAYER_SPEED);
  Vector2 movement = velocity * dt;

  SetPos(GetPos() + movement);

  engine::Packet p(engine::PacketType::INPUT_COMMAND);
  p.Write(GetId());
  p.Write(movement);
  p.Write(GetSpriteTextureId());
  engine::Client::GetInstance().Send(p);
}

} // namespace game
