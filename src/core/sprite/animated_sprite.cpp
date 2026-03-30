#include "core/sprite/animated_sprite.h"

namespace engine {

AnimatedSprite::AnimatedSprite(
      Texture2D texture,
      int texture_id,
      const char* path,
      int cols,
      int rows)
  : Sprite(texture, texture_id, path),
    _cols(cols),
    _rows(rows)
{
  _frame_width = static_cast<double>(texture.width) / _cols;
  _frame_height = static_cast<double>(texture.height) / _rows;
}

AnimatedSprite::AnimatedSprite(
      int texture_id,
      int cols,
      int rows)
  : Sprite(texture_id),
    _cols(cols),
    _rows(rows)
{}

void AnimatedSprite::Draw(Vector2 pos, int col, int row) const
{
  Rectangle frame_rec = {
    col * _frame_width,
    row * _frame_height,
    _frame_width,
    _frame_height
  };

  Sprite::Draw(frame_rec, pos);
}

Rectangle AnimatedSprite::GetSpriteRectangle() 
{
  return (Rectangle) {0.0f, 0.0f, _frame_width, _frame_height};
}

} // namespace engine
