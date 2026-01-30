#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "core/assets/assets_registry.h"
#include "core/sprite/sprite.h"
#include "raylib.h"

namespace engine {

class AnimatedSprite : public Sprite {
public: 
  explicit AnimatedSprite(
      const char* path,
      int cols,
      int rows,
      float frame_speed);

  explicit AnimatedSprite(
      Texture2D texture,
      int cols,
      int rows,
      float frame_speed);

  template<typename T>
  explicit AnimatedSprite(
       AssetRegistry<T> registry,
       T asset_id,
       int cols,
       int rows,
       float frame_speed)
  : Sprite(registry, asset_id),
    _cols(cols),
    _rows(rows),
    _frame_speed(frame_speed),
    _frame_rec{0.0f, 0.0f, 0.0f, 0.0f}
  {}

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
  mutable Rectangle _frame_rec;
};

} // namespace engine

#endif // ANIMATED_SPRITE_H
