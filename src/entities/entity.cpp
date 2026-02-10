#include "entities/entity.h"

namespace engine {

Entity::Entity()
{}

Entity::~Entity()
{
  if (_hitbox)
    delete _hitbox;
}

void Entity::Draw() const 
{
  if (_sprite) 
    _sprite->Draw(_pos);
  if (_hitbox)
    _hitbox->Draw();
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
  if (_hitbox)
    _hitbox->Update(_pos);
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

const Texture2D* Entity::GetTexture() const 
{
  return _sprite->GetTexture();
}

void Entity::SetHitbox(Hitbox* hitbox)
{
  _hitbox = hitbox;
}

std::optional<std::reference_wrapper<const Rectangle>> 
Entity::GetFrameRec() const 
{
  if (AnimatedSprite* d = dynamic_cast<AnimatedSprite*>(_sprite.get()); d != nullptr)
    return std::cref(d->GetCurrentFrameRec());

  return std::nullopt;
}

void Entity::SetHitboxCreationFunction(std::function<Hitbox*()> func) 
{
  _hitbox_creation_function = func;
}
  
const std::function<Hitbox*()> Entity::GetHitboxCreateFuncion() const
{
  return _hitbox_creation_function;
}

} // namespace engine
