#include "node/sprite/animation_player.h"

namespace Shabby::Node {

void AnimationPlayer::Register(
    std::string name, std::weak_ptr<AnimatedSprite> as)
{
  _animations.insert({name, as});
}

void AnimationPlayer::Play(std::string name)
{
  if (auto search = _animations.find(name); 
      search != _animations.end()) 
    if (auto ptr = search->second.lock())
      _current_animation = ptr;
}

void AnimationPlayer::Update(float dt) 
{
  if (_current_animation) {
    if (auto parent = _parent.lock())
      _pos = parent->GetPos();
    _current_animation->Update(dt);
  }
}

void AnimationPlayer::Draw()
{
  if (_current_animation)
    _current_animation->Draw(_pos);
}

Rectangle AnimationPlayer::GetBounds() const
{
  if (_current_animation) {
    return { _pos.x, _pos.y,
             _current_animation->GetFrameWidth(),
             _current_animation->GetFrameHeight() };
  }
  return { _pos.x, _pos.y, 0.f, 0.f };
}

} // namespace Shabby::Node
