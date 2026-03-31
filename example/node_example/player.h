#ifndef PLAYER_H
#define PLAYER_H

#include "node/rigid_body.h"

class Player : public Shabby::Node::RigidBody {
public:
  const float SPEED = 200.0f;

  Player(Vector2 pos)
    :Shabby::Node::RigidBody(pos)
  {}

  void _physics_process(float dt) override
  {
    (void)dt;
    _dir = {0, 0};

    if (IsKeyDown(KEY_A)) _dir.x -= 1 * SPEED;
    if (IsKeyDown(KEY_D)) _dir.x += 1 * SPEED;
    if (IsKeyDown(KEY_W)) _dir.y -= 1 * SPEED;
    if (IsKeyDown(KEY_S)) _dir.y += 1 * SPEED;
  }

  ~Player() = default;
};

#endif // PLAYER_H
