#ifndef ENNEMY_H
#define ENNEMY_H

#include "node/rigid_body.h"
#include <cmath>

class Ennemy : public Shabby::Node::RigidBody {
  public:
    Ennemy(Vector2 center)
      : Shabby::Node::RigidBody(center), _center(center)
    {}

    void _physics_process(float dt) override
    {
      _time += dt;

      float radius = 100.f;

      Vector2 targetPos;
      targetPos.x = _center.x + radius * cosf(_time);
      targetPos.y = _center.y + radius * sinf(_time);

      _dir = { targetPos.x - _pos.x, targetPos.y - _pos.y };
    }

    ~Ennemy() = default;

  private:
    Vector2 _center; 
    float _time = 0.0f;
};

#endif // ENNEMY_H
