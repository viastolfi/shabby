#include "entities/entity.h"

namespace Shabby {

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
  else if (_animation_player)
    _animation_player->Draw(_pos);

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
  if (_animation_player)
    _animation_player->Update();
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

void Entity::SetController(std::unique_ptr<IEntityController> c)
{
  _controller = std::move(c);
}

void Entity::SetVelocity(int velocity)
{
  _velocity = velocity;
}

void Entity::SetSprite(std::shared_ptr<Sprite> sprite)
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

Rectangle Entity::GetSpriteRectangle() const 
{
  if (_sprite != NULL) {
    // TODO: return size of sprite
    return (Rectangle) {0.0f, 0.0f, 0.0f, 0.0f};
  }
  else if (_animation_player != NULL) 
    return _animation_player->GetSpriteRectangle();
  return (Rectangle) {0.0f, 0.0f, 0.0f, 0.0f};
}

void Entity::SetHitboxCreationFunction(std::function<Hitbox*()> func) 
{
  _hitbox_creation_function = func;
}
  
const std::function<Hitbox*()> Entity::GetHitboxCreateFunction() const
{
  return _hitbox_creation_function;
}

Hitbox* Entity::GetHitbox() const 
{
  return _hitbox;
}

void Entity::OnHitboxEntered(Hitbox* enter, Hitbox* from)
{
  if (_controller)
    _controller->OnHitboxEntered(enter, from);
}

void Entity::OnHitboxExited(Hitbox* enter, Hitbox* from)
{
  if (_controller)
    _controller->OnHitboxExited(enter, from);
}

void Entity::PlayAnimation(int index)
{
  if (_animation_player)
    _animation_player->Play(index);
}

void Entity::AddAnimation(Animation& a) 
{
  if (!_animation_player)
    _animation_player = std::make_unique<AnimationPlayer>();

  _animation_player->RegisterAnimation(a);
}

void Entity::SetAnimationPlayer(std::unique_ptr<AnimationPlayer> ap)
{
  _animation_player = std::move(ap);
  _sprite.reset();
}

} // namespace Shabby
