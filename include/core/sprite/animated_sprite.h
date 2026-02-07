#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "core/assets/assets_registry.h"
#include "core/sprite/sprite.h"
#include "raylib.h"

namespace engine {

class AnimatedSprite : public Sprite {
public: 
  explicit AnimatedSprite(
      Texture2D texture,
      int texture_id,
      const char* path,
      int cols,
      int rows,
      float frame_speed);

  explicit AnimatedSprite(
      int texture_id,
      int cols,
      int rows,
      float frame_speed);

  void Load() override;
  void Draw(Vector2 pos) const override;

  int GetFrameCounter() const;
  int GetCurrentFrame() const;
private:
  int _cols;
  int _rows;
  float _frame_speed;
  mutable int _frame_counter = 0;
  mutable int _current_frame = 0;
  mutable Rectangle _frame_rec = {0.0f, 0.0f, 0.0f, 0.0f};
};

} // namespace engine

#endif // ANIMATED_SPRITE_H
