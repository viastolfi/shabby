#ifndef SPRITE_H
#define SPRITE_H

#include "node/inode.h"
#include "raylib.h"

namespace Shabby::Node {

class Sprite : public INode {
public:
  Sprite(Vector2 pos, Texture2D texture);
  ~Sprite() = default;

  void Draw() override;
private:
  Texture2D _texture;
};

} // namespace Shabby::Node

#endif // SPRITE_H
