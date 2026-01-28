#include "entities/entities.h"

namespace engine {

Entity::Entity(std::unique_ptr<Sprite> s) 
  : _sprite(std::move(s)), 
    _velocity(10),
    _pos(Vector2{100.0f, 100.0f})
{}

Entity::Entity(std::unique_ptr<Sprite> s, Vector2 pos)
  : _sprite(std::move(s)),
    _velocity(10),
    _pos(pos)
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
