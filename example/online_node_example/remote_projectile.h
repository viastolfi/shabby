#ifndef REMOTE_PROJECTILE_H
#define REMOTE_PROJECTILE_H

#include "node/inode.h"
#include "node/sprite/animated_sprite.h"

#include "raylib.h"

#include <memory>

// Client-side visual for a projectile.
// Owner (ClientScene) registers the sprite and adds body to the tree.
class RemoteProjectile : public Shabby::Node::INode {
public:
  RemoteProjectile(Vector2 pos, Texture2D tex)
    : INode(pos)
  {
    // EnergyBall.png: 64×16, 4 cols × 1 row
    _sprite = std::make_shared<Shabby::Node::AnimatedSprite>(pos, tex, 0, 3, 0, 0, 4, 1, 12.0f);
  }

  std::shared_ptr<Shabby::Node::AnimatedSprite> GetSprite() const { return _sprite; }

private:
  std::shared_ptr<Shabby::Node::AnimatedSprite> _sprite;
};

#endif // REMOTE_PROJECTILE_H
