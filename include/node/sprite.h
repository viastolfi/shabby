#ifndef SPRITE_H
#define SPRITE_H

#include "node/inode.h"
#include "core/drawable/idrawable.h"
#include "raylib.h"

namespace Shabby::Node {

class Sprite : public INode, public Core::IDrawable {
public:
  Sprite(Vector2 pos, Texture2D texture);
  ~Sprite() = default;

  void Draw() override final;
  void Update(float dt) override final;
private:
  Texture2D _texture;
};

} // namespace Shabby::Node

#endif // SPRITE_H
