#ifndef REMOTE_ENEMY_H
#define REMOTE_ENEMY_H

#include "node/inode.h"
#include "node/sprite/sprite.h"

#include "raylib.h"

#include <memory>

// Client-side visual representation of an enemy.
// Does NOT register with RenderSystem or call AddChild — the owner
// (ClientScene) performs both operations after construction.
class RemoteEnemy : public Shabby::Node::INode {
public:
  RemoteEnemy(Vector2 pos, Texture2D texture)
    : INode(pos)
  {
    _sprite = std::make_shared<Shabby::Node::Sprite>(pos, texture);
  }

  ~RemoteEnemy() = default;

  std::shared_ptr<Shabby::Node::Sprite> GetSprite() const { return _sprite; }

private:
  std::shared_ptr<Shabby::Node::Sprite> _sprite;
};

#endif // REMOTE_ENEMY_H
