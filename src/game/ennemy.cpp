#include "ennemy.h"

namespace game {

Ennemy::Ennemy(std::unique_ptr<engine::Sprite> s)
  :Entity(std::move(s))
{}

Ennemy::Ennemy(std::unique_ptr<engine::Sprite> s, Vector2 pos)
  : Entity(std::move(s), pos)
{}

void Ennemy::Update(float dt)
{
  Entity::Update(dt);
}

} // namespace game
