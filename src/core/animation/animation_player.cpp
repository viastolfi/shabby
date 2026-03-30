#include "core/animation/animation_player.h"

namespace Shabby {

void AnimationPlayer::Play(int index) 
{
  if (index < 0 || index >= static_cast<int>(_animations.size())) return;
  _current_animation = _animations[index];
}

void AnimationPlayer::Draw(Vector2 pos)
{
  if (_current_animation)
    _current_animation->Draw(pos);
}

void AnimationPlayer::Update()
{
  if (_current_animation)
    _current_animation->Update();
}

Rectangle AnimationPlayer::GetSpriteRectangle()
{
  if (_current_animation) 
    return _current_animation->GetSpriteRectangle();

  return {};
}

void AnimationPlayer::RegisterAnimation(Animation& a)
{
  _animations.push_back(&a);
}

} // namespace Shabby
