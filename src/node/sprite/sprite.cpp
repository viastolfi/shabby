#include "node/sprite/sprite.h"

namespace Shabby::Node {

Sprite::Sprite(Vector2 pos, Texture2D texture)
  :INode(pos), _texture(texture)
{}

void Sprite::Draw() 
{
  DrawTextureV(_texture, _pos, WHITE);
}

Rectangle Sprite::GetBounds() const
{
  return { _pos.x, _pos.y, (float)_texture.width, (float)_texture.height };
}

void Sprite::Update(float dt)
{
  (void)dt;
  if (auto parent = _parent.lock())
    _pos = parent->GetPos();
}

} 
