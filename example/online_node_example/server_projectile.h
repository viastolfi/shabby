#ifndef SERVER_PROJECTILE_H
#define SERVER_PROJECTILE_H

#include "node/inode.h"
#include "raylib.h"

class ServerProjectile : public Shabby::Node::INode {
public:
  static constexpr float SIZE = 8.0f;

  ServerProjectile(Vector2 pos, Vector2 vel)
    : Shabby::Node::INode(pos), _vel(vel)
  {}

  void Update(float dt) override
  {
    _pos.x += _vel.x * dt;
    _pos.y += _vel.y * dt;
    Shabby::Node::INode::Update(dt);
  }

private:
  Vector2 _vel;
};

#endif // SERVER_PROJECTILE_H
