#ifndef PLAYER_H
#define PLAYER_H

#include "entities/entities.h"
#include "networking/packet.h"
#include "raymath.h"

namespace game {

class Player : public engine::Entity {
public:
  explicit Player(std::unique_ptr<engine::Sprite> s);
  explicit Player(std::unique_ptr<engine::Sprite> s, Vector2 pos);
  explicit Player(Vector2 pos, size_t id = 0);
  ~Player() override = default;
  
  void Update(float dt) override;
  
private:
  static constexpr float PLAYER_SPEED = 200.0f;
};

} // namespace game

#endif // PLAYER_H
