#include "entities/entities.h"

namespace engine {

Entity::Entity(std::unique_ptr<Sprite> s, size_t id) 
  : _id(id),
    _sprite(std::move(s)), 
    _velocity(10),
    _pos(Vector2{100.0f, 100.0f})
{}

Entity::Entity(std::unique_ptr<Sprite> s, Vector2 pos, size_t id)
  : _id(id),
    _sprite(std::move(s)),
    _velocity(10),
    _pos(pos)
{}

Entity::Entity(Vector2 pos, size_t id)
  : _id(id),
    _sprite(nullptr),
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

void Entity::Update(float dt)
{
  (void)(dt);
}

void Entity::Draw() const 
{
  if (_sprite) 
    _sprite->Draw(_pos);
}

size_t Entity::GetId() const
{
  return _id;
}

void Entity::SetId(size_t id)
{
  _id = id;
}

const char* Entity::GetSpritePath() const
{
  if (!_sprite) return nullptr;
  return _sprite->GetPath();
}

int Entity::GetSpriteTextureId() const
{
  if (!_sprite) return 0;
  return _sprite->GetTextureId();
}
  
void Entity::LoadSprite() const
{
  if (_sprite) {
    _sprite->Load();
  }
}

} // namespace engine
