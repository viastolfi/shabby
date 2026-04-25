#include "node/sprite/animated_sprite.h"

namespace Shabby::Node {

AnimatedSprite::AnimatedSprite(
    Vector2 pos,
    Texture2D texture,
    int sc, int ec, int sr, int er,
    int total_cols, int total_rows,
    float fs)
  :INode(pos), _texture(texture), _start_col(sc),
   _end_col(ec), _start_row(sr), _end_row(er),
   _frame_speed(fs)
{
  _current_col = _start_col;
  _current_row = _start_row;
  _frame_width = _texture.width / total_cols;
  _frame_height = _texture.height / total_rows;
  _frame_rec = Rectangle{
    static_cast<float>(_start_col * _frame_width),
    static_cast<float>(_start_row * _frame_height),
    static_cast<float>(_frame_width),
    static_cast<float>(_frame_height)
  };
}

void AnimatedSprite::Update(float dt)
{
  // Sync position from parent (consistent with Sprite behaviour).
  if (auto parent = _parent.lock())
    _pos = parent->GetPos();

  _frame_counter += dt;

  float time_per_frame = 1.0f / _frame_speed;  

  if (_frame_counter >= time_per_frame) {
    _frame_counter -= time_per_frame;

    _current_col++;
    if (_current_col > _end_col) {
      _current_col = _start_col;
      _current_row++;
      if (_current_row > _end_row)
        _current_row = _start_row;
    }

    _frame_rec.x = static_cast<float>(_current_col * _frame_width);
    _frame_rec.y = static_cast<float>(_current_row * _frame_height);
  }
}

void AnimatedSprite::Draw() 
{
  DrawTextureRec(_texture, _frame_rec, _pos, WHITE);
}

void AnimatedSprite::Draw(Vector2 pos) 
{
  DrawTextureRec(_texture, _frame_rec, pos, WHITE);
}

Rectangle AnimatedSprite::GetBounds() const
{
  return { _pos.x, _pos.y, _frame_width, _frame_height };
}

} // namespace Shabby::Node
