#include "core/animation/animation.h"

namespace Shabby {

Animation::Animation(std::shared_ptr<AnimatedSprite> sp)
  :_sprite_sheet(std::move(sp))
{}

Animation::Animation(
      std::shared_ptr<AnimatedSprite> sp,
      int sc, int ec, int sr, int er, float fs)
  :_sprite_sheet(std::move(sp)),
   _start_col(sc), _end_col(ec),
   _start_row(sr), _end_row(er),
   _frame_speed(fs),
   _current_col(sc), _current_row(sr)
{}

void Animation::Update() 
{
  _frame_counter++; 

  // TODO: make this dependant on the wanted amount of fps
  if (_frame_counter >= (60/_frame_speed)) {
    _frame_counter = 0; 

    _current_col++;
    if (_current_col > _end_col) {
      _current_col = _start_col;
      _current_row++;
      if (_current_row > _end_row)
        _current_row = _start_row;
    }
  }
}

Rectangle Animation::GetSpriteRectangle()
{
  return _sprite_sheet->GetSpriteRectangle();
}

void Animation::Draw(Vector2 pos)
{
  _sprite_sheet->Draw(pos, _current_col, _current_row);
}

} // namespace Shabby
