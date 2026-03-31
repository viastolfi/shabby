#include "node/sprite.h"

namespace Shabby::Node {

Sprite::Sprite(Vector2 pos, Texture2D texture)
  :INode(pos), _texture(texture)
{}

void Sprite::Draw() 
{
  DrawTextureV(_texture, _pos, WHITE);
}

} 
