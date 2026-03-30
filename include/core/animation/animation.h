#ifndef ANIMATION_H
#define ANIMATION_H

#include <memory>

#include "core/sprite/animated_sprite.h"
#include "raylib.h"

namespace engine {

class Animation {
public:
  Animation(std::shared_ptr<AnimatedSprite> sp);
  Animation(
      std::shared_ptr<AnimatedSprite> sp,
      int sc, int ec, int sr, int er, float fs);
  ~Animation() = default;

  void Update();
  void Draw(Vector2 pos);
  Rectangle GetSpriteRectangle();
private:
  std::shared_ptr<AnimatedSprite> _sprite_sheet;
  int _start_col = 0;
  int _end_col = 0;
  int _start_row = 0;
  int _end_row = 0;
  float _frame_speed = 0.0f;

  int _frame_counter = 0;
  int _current_col = 0;
  int _current_row = 0;
};

} // namespace engine

#endif // ANIMATION_H
