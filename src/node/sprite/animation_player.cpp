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

} // namespace Shabby::Node
