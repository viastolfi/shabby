#include "core/sprite/sprite.h"

namespace engine {

Sprite::Sprite(const char* path) 
  : _loaded(false),
    _path(path)
{}

Sprite::Sprite(std::string path)
  : _loaded(false),
    _path(path.c_str())
{}

Sprite::Sprite(Texture2D texture)
  : _texture(texture),
    _loaded(true)
{}
     
Sprite::Sprite(Texture2D texture, const char* path)
  : _texture(texture),
    _loaded(true),
    _path(path)
{}

Sprite::~Sprite()
{
  if (_loaded) 
    UnloadTexture(_texture);
}

Sprite::Sprite(Sprite&& other) noexcept 
  : _texture(other._texture), 
    _loaded(other._loaded),
    _path(other._path)
{
  other._loaded = false;
  other._path = nullptr;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept 
{
  if (this != &other) {
    if (_loaded) 
      UnloadTexture(_texture);

    _texture = other._texture;
    _loaded = other._loaded;
    _path = other._path;
    _path = other._path;
    other._loaded = false;
    other._path = nullptr;
  }
  return *this;
}

void Sprite::Load()
{
  if (_loaded) {
    return;
  }
  _texture = LoadTexture(_path);
  _loaded = true;
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

void Sprite::Draw(Rectangle frame_rec, Vector2 pos) const 
{
  if (_loaded)
    DrawTextureRec(_texture, frame_rec, pos, WHITE);
}

const char* Sprite::GetPath() const
{
  return _path;
}

} // namespace engine
