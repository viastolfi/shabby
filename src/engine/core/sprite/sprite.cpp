#include "sprite.h"

namespace engine {

Sprite::Sprite(const char* path) 
  : _loaded(false) 
{
  _texture = LoadTexture(path);
  _loaded = true;
}

Sprite::~Sprite() 
{
  if (_loaded) 
    UnloadTexture(_texture);
}

Sprite::Sprite(Sprite&& other) noexcept 
  : _texture(other._texture), 
    _loaded(other._loaded) 
{
  other._loaded = false;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept 
{
  if (this != &other) {
    if (_loaded) 
      UnloadTexture(_texture);

    _texture = other._texture;
    _loaded = other._loaded;
    other._loaded = false;
  }
  return *this;
}

void Sprite::Draw(float x, float y) const 
{
  if (_loaded) 
    DrawTexture(
        _texture, 
        static_cast<int>(x), 
        static_cast<int>(y), 
        WHITE);
}

void Sprite::Draw(Vector2 pos) const
{
  if (_loaded)
    DrawTextureV(_texture, pos, WHITE);
}

} // namespace engine
