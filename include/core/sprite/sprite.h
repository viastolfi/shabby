#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include "raylib.h"

namespace engine {

class Sprite {
public:
  explicit Sprite(const char* path);
  explicit Sprite(std::string path);
  virtual ~Sprite(); 
  
  // remove copy
  Sprite(const Sprite&) = delete;
  Sprite& operator=(const Sprite&) = delete;
  
  // accept move
  Sprite(Sprite&& other) noexcept;
  Sprite& operator=(Sprite&& other) noexcept;
  
  virtual void Load();
  virtual void Draw(float x, float y) const;
  virtual void Draw(Vector2 pos) const;  
  virtual void Draw(Rectangle frame_rec, Vector2 pos) const;

  const char* GetPath() const;
protected:
  const Texture2D& GetTexture() const { return _texture; }

private:
  Texture2D _texture;
  bool _loaded;
  const char* _path;
};

} // namespace engine

#endif // SPRITE_H
