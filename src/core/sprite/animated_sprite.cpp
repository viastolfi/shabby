#include "core/sprite/animated_sprite.h"

namespace engine {

AnimatedSprite::AnimatedSprite(
    const char* path,
    int cols,
    int rows,
    float frame_speed)
  : Sprite(path), 
    _cols(cols), 
    _rows(rows), 
    _frame_speed(frame_speed),
    _frame_rec{0.0f, 0.0f, 0.0f, 0.0f}
{}

void AnimatedSprite::Load()
{
  Sprite::Load();
  _frame_rec.width = static_cast<float>(GetTexture().width) / _cols;
  _frame_rec.height = static_cast<float>(GetTexture().height);
}

int AnimatedSprite::GetFrameCounter() const
{
  return _frame_counter;
}

int AnimatedSprite::GetCurrentFrame() const
{
  return _current_frame; 
}

void AnimatedSprite::Draw(Vector2 pos) const
{
  _frame_counter++;

  if (_frame_counter >= (60/_frame_speed)) {
    _frame_counter = 0;
    _current_frame++;

    if (_current_frame > _cols - 1)
      _current_frame = 0;

    _frame_rec.x = 
      static_cast<float>(_current_frame) *
      static_cast<float>(GetTexture().width) /
      _cols;
  }

  Sprite::Draw(_frame_rec, pos);
}

} // namespace engine
