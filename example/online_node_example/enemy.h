#ifndef ONLINE_ENEMY_H
#define ONLINE_ENEMY_H

#include "node/collision_shape/rigid_body.h"
#include <cmath>

class Enemy : public Shabby::Node::RigidBody {
public:
  Enemy(Vector2 center, bool is_authority)
    :Shabby::Node::RigidBody(center), _is_authority(is_authority), _center(center)
  {}

  ~Enemy() = default;

  void _physics_process(float dt) override
  {
    if (_is_authority) {
      _time += dt;
      float radius = 100.0f;

      Vector2 target;
      target.x = _center.x + radius * cosf(_time);
      target.y = _center.y + radius * sinf(_time);

      _dir = {target.x - _pos.x, target.y - _pos.y};
    } else {
      _dir = {0, 0};
    }
  }

private:
  bool _is_authority;
  Vector2 _center;
  float _time = 0;
};

#endif // ONLINE_ENEMY_H
