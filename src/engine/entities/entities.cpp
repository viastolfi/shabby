#include "entities.h"

namespace engine {

Entity::Entity(const char* texture_path) 
  : _sprite(std::make_unique<AnimatedSprite>(texture_path,4,1,1)), 
    _velocity(10),
    _pos(Vector2{100.0f, 100.0f})
{}

Vector2 Entity::GetPos() const
{
  return _pos;
}

void Entity::SetPos(Vector2 p) 
{
  _pos = p;
}

void Entity::Update(float dt) {}

void Entity::Draw() const 
{
  if (_sprite) 
    _sprite->Draw(_pos);
}

} // namespace engine
