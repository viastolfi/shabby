#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "node/inode.h"
#include "core/drawable/idrawable.h"

#include "raylib.h"

namespace Shabby::Node {

class AnimatedSprite : public INode, public Core::IDrawable {
public:
  AnimatedSprite(
      Vector2 pos,
      Texture2D texture,
      int sc, int ec, int sr, int er,
      int total_cols, int total_rows,
      float fs);
  ~AnimatedSprite() = default;

  void Draw() override final;
  void Draw(Vector2 pos) override final;
  void Update(float dt) override final;
private:
  Texture _texture;
  int _start_col;
  int _end_col;
  int _start_row;
  int _end_row;
  int _current_col;
  int _current_row;
  float _frame_width;
  float _frame_height;
  Rectangle _frame_rec;
  float _frame_speed;
  float _frame_counter = 0.0f;
};

} // namespace Shabby::Node

#endif // ANIMATED_SPRITE_H
