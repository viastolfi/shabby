#ifndef PLAYER_H
#define PLAYER_H

#include "engine/entities/entities.h"
#include "raylib.h"

namespace game {

class Player : public engine::Entity {
public:
  explicit Player(const char* texture_path);
  ~Player() override = default;
  
  void Update(float dt) override;
  
private:
  static constexpr float PLAYER_SPEED = 200.0f;
};

} // namespace game

#endif // PLAYER_H
