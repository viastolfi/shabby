#ifndef ENNEMY_H
#define ENNEMY_H

#include "engine/entities/entities.h"

namespace game {

class Ennemy : public engine::Entity {
public:
  explicit Ennemy(std::unique_ptr<engine::Sprite> s);
  explicit Ennemy(std::unique_ptr<engine::Sprite> s, Vector2 pos);
  ~Ennemy() override = default;

  void Update(float dt) override;
};

} // namespace game

#endif // ENNEMY_H
