#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include "core/assets/assets_registry.h"
#include "raylib.h"

namespace engine {

class Sprite {
public:
  explicit Sprite(const char* path);
  explicit Sprite(std::string path);
  explicit Sprite(Texture2D texture);
  explicit Sprite(Texture2D texture, const char* path);
  explicit Sprite(int texture_id);

  template<typename T>
  explicit Sprite(
      AssetRegistry<T> registry,
      T asset_id)
  : _texture(registry.GetTexture(asset_id)),
    _texture_id(static_cast<int>(asset_id)),
    _loaded(true),
    _path(registry.GetTexturePath(asset_id))
  {}

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
  int GetTextureId() const;
protected:
  const Texture2D& GetTexture() const { return _texture; }

private:
  Texture2D _texture;
  int _texture_id = 0;
  bool _loaded;
  const char* _path;
};

} // namespace engine

#endif // SPRITE_H
