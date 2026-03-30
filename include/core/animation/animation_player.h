#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H

#include <vector>

#include "core/animation/animation.h"
#include "raylib.h"

namespace Shabby {

class AnimationPlayer {
public:
  AnimationPlayer() = default;
  ~AnimationPlayer() = default;

  void Play(int index);
  void Draw(Vector2 pos);
  void Update();
  Rectangle GetSpriteRectangle();

  void RegisterAnimation(Animation& a);
private:
  std::vector<Animation*> _animations;
  Animation* _current_animation = NULL;
};

} // namespace Shabby

#endif // ANIMATION_PLAYER_H
