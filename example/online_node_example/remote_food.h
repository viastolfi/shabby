#ifndef REMOTE_FOOD_H
#define REMOTE_FOOD_H

#include "node/inode.h"
#include "node/sprite/sprite.h"

#include "raylib.h"

#include <memory>

// Client-side visual for a food pickup.
// Owner (ClientScene) registers the sprite and adds body to the tree.
class RemoteFood : public Shabby::Node::INode {
public:
  RemoteFood(Vector2 pos, Texture2D tex)
    : INode(pos)
  {
    _sprite = std::make_shared<Shabby::Node::Sprite>(pos, tex);
  }

  std::shared_ptr<Shabby::Node::Sprite> GetSprite() const { return _sprite; }

private:
  std::shared_ptr<Shabby::Node::Sprite> _sprite;
};

#endif // REMOTE_FOOD_H
