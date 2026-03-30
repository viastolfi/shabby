#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "core/assets/assets_registry.h"
#include "core/sprite/sprite.h"
#include "raylib.h"

namespace Shabby {

class AnimatedSprite : public Sprite {
public: 
  explicit AnimatedSprite(
      Texture2D texture,
      int texture_id,
      const char* path,
      int cols,
      int rows);

  explicit AnimatedSprite(
      int texture_id,
      int cols,
      int rows);

  void Draw(Vector2 pos, int col, int row) const override;

  Rectangle GetSpriteRectangle();
private:
  int _cols;
  int _rows;

  float _frame_width;
  float _frame_height; 
};

} // namespace Shabby

#endif // ANIMATED_SPRITE_H
