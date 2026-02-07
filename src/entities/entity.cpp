#include "entities/entity.h"

namespace engine {

Entity::Entity()
{}

void Entity::Draw() const 
{
  if (_sprite) 
    _sprite->Draw(_pos);
}

void Entity::Init()
{
  if (_controller)
    _controller->OnInit(this);
}

void Entity::Update(float dt)
{
  if (_controller)
    _controller->OnUpdate(this, dt);
}

const char* Entity::GetSpritePath() const
{
  if (!_sprite) return nullptr;
  return _sprite->GetPath();
}

int Entity::GetSpriteTextureId() const
{
  if (!_sprite) return _texture_id;
  return _sprite->GetTextureId();
}
  
void Entity::LoadSprite() const
{
  if (_sprite) {
    _sprite->Load();
  }
}

void Entity::SetController(std::unique_ptr<IEntityController> c)
{
  _controller = std::move(c);
}

void Entity::SetVelocity(int velocity)
{
  _velocity = velocity;
}

void Entity::SetSprite(std::unique_ptr<Sprite> sprite)
{
  _sprite = std::move(sprite);
}

} // namespace engine
