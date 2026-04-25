#ifndef SERVER_ENEMY_H
#define SERVER_ENEMY_H

#include "node/collision_shape/rigid_body.h"
#include <cmath>

class ServerEnemy : public Shabby::Node::RigidBody {
public:
  ServerEnemy(Vector2 center)
    : Shabby::Node::RigidBody(center), _center(center)
  {}

  ~ServerEnemy() = default;

  void _physics_process(float dt) override
  {
    _time += dt;
    constexpr float radius = 100.0f;
    Vector2 target;
    target.x = _center.x + radius * cosf(_time);
    target.y = _center.y + radius * sinf(_time);
    _dir = {target.x - _pos.x, target.y - _pos.y};
  }

private:
  Vector2 _center;
  float   _time = 0;
};

#endif // SERVER_ENEMY_H
