#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"

namespace engine {

class Sprite {
public:
  explicit Sprite(const char* path);
  ~Sprite();
  
  // remove copy
  Sprite(const Sprite&) = delete;
  Sprite& operator=(const Sprite&) = delete;
  
  // accept move
  Sprite(Sprite&& other) noexcept;
  Sprite& operator=(Sprite&& other) noexcept;
  
  void Draw(float x, float y) const;
  void Draw(Vector2 pos) const;
  
private:
  Texture2D _texture;
  bool _loaded;
};

} // namespace engine

#endif // SPRITE_H
