#include "entities.h"

namespace engine {

Entity::Entity(const char* texture_path) 
  : _sprite(std::make_unique<Sprite>(texture_path)), 
    _velocity(10),
    _pos(Vector2{100.0f, 100.0f})
{}

void Entity::Update(float dt) {}

void Entity::Draw() const 
{
  if (_sprite) 
    _sprite->Draw(_pos);
}

Vector2 Entity::GetPos()
{
  return _pos;
}

void Entity::SetPos(Vector2 p)
{
  _pos = p;
}

} // namespace engine
